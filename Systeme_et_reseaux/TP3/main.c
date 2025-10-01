#define FUSE_USE_VERSION 26

#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "tosfs.h"
#include <fuse/fuse_lowlevel.h>


struct tosfs_superblock* superblock;
struct tosfs_inode* inodes;
struct tosfs_dentry* dentries;

static int tosfs_stat(fuse_ino_t ino, struct stat *stbuf)
{
	// Find the inode in our filesystem
	if (ino == TOSFS_ROOT_INODE) {
		// Root directory
		stbuf->st_ino = ino;
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		stbuf->st_uid = 0;
		stbuf->st_gid = 0;
		stbuf->st_size = TOSFS_BLOCK_SIZE;
		stbuf->st_blksize = TOSFS_BLOCK_SIZE;
		stbuf->st_blocks = 1;
		return 0;
	}
	
	// Look for the inode in our inode list
	for (int i = 0; i < superblock->inodes; i++) {
		if (inodes[i].inode == ino) {
			stbuf->st_ino = ino;
			stbuf->st_mode = inodes[i].mode;
			stbuf->st_nlink = inodes[i].nlink;
			stbuf->st_uid = inodes[i].uid;
			stbuf->st_gid = inodes[i].gid;
			stbuf->st_size = inodes[i].size;
			stbuf->st_blksize = TOSFS_BLOCK_SIZE;
			stbuf->st_blocks = (inodes[i].size + 511) / 512;
			return 0;
		}
	}
	return -1;
}

static void tosfs_getattr(fuse_req_t req, fuse_ino_t ino,
			     struct fuse_file_info *fi)
{
	struct stat stbuf;

	(void) fi;

	memset(&stbuf, 0, sizeof(stbuf));
	if (tosfs_stat(ino, &stbuf) == -1)
		fuse_reply_err(req, ENOENT);
	else
		fuse_reply_attr(req, &stbuf, 1.0);
}

static void tosfs_lookup(fuse_req_t req, fuse_ino_t parent, const char *name){
}


static void tosfs_readdir(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi)
{

}

static void tosfs_open(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi)
{
}

static void tosfs_read(fuse_req_t req, fuse_ino_t ino, size_t size,
			  off_t off, struct fuse_file_info *fi)
{
}
static struct fuse_lowlevel_ops oper = {
	.lookup		= tosfs_lookup,
	.getattr	= tosfs_getattr,
	.readdir	= tosfs_readdir,
	.open		= tosfs_open,
	.read		= tosfs_read,
};

int main(int argc, char* argv[]) {
	int fd = open("./test_tosfs_files", O_RDWR);
	if (fd == -1) {
		perror("Error opening file");
		return 1;
	}

	struct stat stats;
	if (fstat(fd, &stats) == -1) {
		perror("Could not access file information");
		close(fd);
		return 1;
	}

	void *map = mmap(NULL, stats.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED) {
		perror("Could not map file");
		close(fd);
		return 1;
	}

	// Map the filesystem structures
	superblock = (struct tosfs_superblock *)map;
	inodes = (struct tosfs_inode *)(map + TOSFS_INODE_BLOCK * TOSFS_BLOCK_SIZE);
	dentries = (struct tosfs_dentry *)(map + TOSFS_ROOT_BLOCK * TOSFS_BLOCK_SIZE);

    // printf("=== Superblock ===\n");
    // printf("magic: 0x%x\n", superblock->magic);
    // printf("block_size: %u\n bytes", superblock->block_size);
    // printf("blocks: %u\n", superblock->blocks);
    // printf("inodes: %u\n", superblock->inodes);
    // printf("root_inode: %u\n", superblock->root_inode);
    // printf("block_bitmap: "PRINTF_BINARY_PATTERN_INT32"\n", PRINTF_BYTE_TO_BINARY_INT32(superblock->block_bitmap));
    // printf("inode_bitmap: "PRINTF_BINARY_PATTERN_INT32"\n", PRINTF_BYTE_TO_BINARY_INT32(superblock->inode_bitmap));
    // printf("\n");
	// for (int i = 0; i < superblock->inodes; i++) {
    //     if (inodes[i].inode != 0) {
    //         printf("=== Inode ===\n");
	// 		printf("inode: %u\n", inodes[i].inode);
	// 		printf("block_no: %u\n", inodes[i].block_no);
	// 		printf("uid: %u\n", inodes[i].uid);
	// 		printf("gid: %u\n", inodes[i].gid);
	// 		printf("mode: %u\n", inodes[i].mode);
	// 		printf("permission: %u\n", inodes[i].perm);
	// 		printf("size: %u bytes\n", inodes[i].size);
	// 		printf("nlink: %u\n", inodes[i].nlink);
	// 		printf("\n");
	// 	}
	// }
	// for (int i = 0; i < superblock->inodes; i++) {
	// 	if (dentries[i].inode != 0) {
	// 		printf("=== Dentry ===\n");
	// 		printf("inode: %u\n", dentries[i].inode);
	// 		printf("name: %s\n", dentries[i].name);
	// 		printf("\n");
	// 	}
	// }


	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
	struct fuse_chan *ch;
	char *mountpoint;
	int err = -1;

	
	if (fuse_parse_cmdline(&args, &mountpoint, NULL, NULL) != -1 &&
	(ch = fuse_mount(mountpoint, &args)) != NULL) {
		struct fuse_session *se;
		

		se = fuse_lowlevel_new(&args, &oper,
					   sizeof(oper), NULL);
		if (se != NULL) {
			if (fuse_set_signal_handlers(se) != -1) {
				fuse_session_add_chan(se, ch);
				err = fuse_session_loop(se);
				fuse_remove_signal_handlers(se);
				fuse_session_remove_chan(ch);
			}
			fuse_session_destroy(se);
		}
		fuse_unmount(mountpoint, ch);
		}
	fuse_opt_free_args(&args);

	// Cleanup
	munmap(map, stats.st_size);
	close(fd);
	return err ? 1 : 0;
}