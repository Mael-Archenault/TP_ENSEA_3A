#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "tosfs.h"
#include <fuse/fuse_lowlevel.h>

#define _FILE_OFFSET_BITS 64

/*static struct fuse_lowlevel_ops oper = {
	.lookup		= hello_ll_lookup,
	.getattr	= hello_ll_getattr,
	.readdir	= hello_ll_readdir,
	.open		= hello_ll_open,
	.read		= hello_ll_read,
};*/

int main() {
	int fd = open("../test_tosfs_files", O_RDWR);
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

	void *map = mmap(NULL,stats.st_size,PROT_READ,MAP_PRIVATE,fd,0);
	if (map == MAP_FAILED) {
		perror("Could not map file");
		close(fd);
		return 1;
	}

	struct tosfs_superblock *superblock = (struct tosfs_superblock *)map;
	printf("=== Superblock ===\n");
	printf("magic: 0x%x\n", superblock->magic);
	printf("block_size: %u\n bytes", superblock->block_size);
	printf("blocks: %u\n", superblock->blocks);
	printf("inodes: %u\n", superblock->inodes);
	printf("root_inode: %u\n", superblock->root_inode);
	printf("block_bitmap: "PRINTF_BINARY_PATTERN_INT32"\n", PRINTF_BYTE_TO_BINARY_INT32(superblock->block_bitmap));
	printf("inode_bitmap: "PRINTF_BINARY_PATTERN_INT32"\n", PRINTF_BYTE_TO_BINARY_INT32(superblock->inode_bitmap));
	printf("\n");

	struct tosfs_inode *inodes = (struct tosfs_inode *)(map + TOSFS_INODE_BLOCK * TOSFS_BLOCK_SIZE); // Mapping blocks after superblock
	for (int i = 0; i < superblock->inodes; i++) {
		if (inodes[i].inode != 0) {
			printf("=== Inode ===\n");
			printf("inode: %u\n", inodes[i].inode);
			printf("block_no: %u\n", inodes[i].block_no);
			printf("uid: %u\n", inodes[i].uid);
			printf("gid: %u\n", inodes[i].gid);
			printf("mode: %u\n", inodes[i].mode);
			printf("permission: %u\n", inodes[i].perm);
			printf("size: %u bytes\n", inodes[i].size);
			printf("nlink: %u\n", inodes[i].nlink);
			printf("\n");
		}
	}

	struct tosfs_dentry *dentries = (struct tosfs_dentry *)(map + TOSFS_ROOT_BLOCK * TOSFS_BLOCK_SIZE);
	for (int i = 0; i < superblock->inodes; i++) {
		if (dentries[i].inode != 0) {
			printf("=== Dentry ===\n");
			printf("inode: %u\n", dentries[i].inode);
			printf("name: %s\n", dentries[i].name);
			printf("\n");
		}
	}


/*	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
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

	return err ? 1 : 0;
*/

	return 0;
}