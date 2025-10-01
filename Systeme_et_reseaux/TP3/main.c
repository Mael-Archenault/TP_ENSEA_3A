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
#define FUSE_USE_VERSION 26
#include <fuse/fuse_lowlevel.h>
#include "tosfs.h"
#include <math.h>

#define min(x, y) ((x) < (y) ? (x) : (y))

void* map;
struct tosfs_superblock* superblock;
struct tosfs_inode* inodes;
struct tosfs_dentry* dentries;

//////////////////////////// useful functions //////////////////////////////////
void print_state(){
	printf("=== Superblock ===\n");
    printf("magic: 0x%x\n", superblock->magic);
    printf("block_size: %u bytes\n", superblock->block_size);
    printf("blocks: %u\n", superblock->blocks);
    printf("inodes: %u\n", superblock->inodes);
    printf("root_inode: %u\n", superblock->root_inode);
    printf("block_bitmap: "PRINTF_BINARY_PATTERN_INT32"\n", PRINTF_BYTE_TO_BINARY_INT32(superblock->block_bitmap));
    printf("inode_bitmap: "PRINTF_BINARY_PATTERN_INT32"\n", PRINTF_BYTE_TO_BINARY_INT32(superblock->inode_bitmap));
    printf("\n");
	for (int i = 0; i < superblock->inodes + 1; i++) {
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
	for (int i = 0; i < superblock->inodes + 1; i++) {
		printf("=== Dentry ===\n");
		printf("inode: %u\n", dentries[i].inode);
		printf("name: %s\n", dentries[i].name);
		printf("\n");
	}
}


int get_inode_with_id(struct tosfs_inode** res, int inode_number){
	printf("Searching inode with number: %d\n\r", inode_number);
	
	// Safety checks
	if (res == NULL) {
		printf("ERROR: res pointer is NULL\n");
		return -1;
	}
	
	if (superblock == NULL) {
		printf("ERROR: superblock is NULL\n");
		return -1;
	}
	
	if (inodes == NULL) {
		printf("ERROR: inodes is NULL\n");
		return -1;
	}
	
	// Fixed bounds: should be < superblock->inodes, not <= 
	for (int i = 0; i < superblock->inodes + 1; i++) {
		if (inodes[i].inode == inode_number) {
			printf("Found inode at index %d\n\r", i);
			*res = &inodes[i];
			return 0;
		}
	}
	
	printf("Inode %d not found\n", inode_number);
	print_state();
	return -1;
}
static int tosfs_stat(fuse_ino_t ino, struct stat *stbuf)
{
	struct tosfs_inode* current_inode;
	if(get_inode_with_id(&current_inode, ino)==-1){
		return -1;
	}

	stbuf->st_ino = ino;
	stbuf->st_mode = current_inode->mode;
	stbuf->st_nlink = current_inode->nlink;
	stbuf->st_uid = current_inode->uid;
	stbuf->st_gid = current_inode->gid;
	stbuf->st_size = current_inode->size;
	stbuf->st_blksize = TOSFS_BLOCK_SIZE;
	stbuf->st_blocks = (current_inode->size + 511) / 512;
	return 0;
}

struct dirbuf {
	char *p;
	size_t size;
};

static void dirbuf_add(fuse_req_t req, struct dirbuf *b, const char *name,
		       fuse_ino_t ino)
{
	struct stat stbuf;
	size_t oldsize = b->size;
	b->size += fuse_add_direntry(req, NULL, 0, name, NULL, 0);
	b->p = (char *) realloc(b->p, b->size);
	memset(&stbuf, 0, sizeof(stbuf));
	stbuf.st_ino = ino;
	fuse_add_direntry(req, b->p + oldsize, b->size - oldsize, name, &stbuf,
			  b->size);
}

static int reply_buf_limited(fuse_req_t req, const char *buf, size_t bufsize,
			     off_t off, size_t maxsize)
{
	if (off < bufsize)
		return fuse_reply_buf(req, buf + off,
				      min(bufsize - off, maxsize));
	else
		return fuse_reply_buf(req, NULL, 0);
}



//////////////////////////// Fuse functions implementations //////////////////////////////////

// getattr-> gives all the inode informations of a file
static void tosfs_getattr(fuse_req_t req, fuse_ino_t ino,struct fuse_file_info *fi)
{
	struct stat stbuf;
	memset(&stbuf, 0, sizeof(stbuf));
	if (tosfs_stat(ino, &stbuf) == -1)
		fuse_reply_err(req, ENOENT);
	else
		fuse_reply_attr(req, &stbuf, 1.0);
}

// lookup-> searches for a file with a specific name inside of a directory
static void tosfs_lookup(fuse_req_t req, fuse_ino_t parent, const char *name){
	struct tosfs_inode* parent_inode;
	if(get_inode_with_id(&parent_inode, parent) == -1){
		fuse_reply_err(req, ENOENT);
		return;
	};

	int block_no = parent_inode->block_no;
	int n_link = parent_inode->nlink;
	struct tosfs_dentry* dentry_table = (struct tosfs_dentry*)(map + TOSFS_BLOCK_SIZE*block_no);
	for(int i = 0; i<n_link; i++){
		if (strcmp(dentry_table[i].name, name)==0){
			struct fuse_entry_param e;
			memset(&e, 0, sizeof(e));
			e.ino = dentry_table[i].inode;
			e.attr_timeout = 1.0;
			e.entry_timeout = 1.0;
			tosfs_stat(e.ino, &e.attr);
			fuse_reply_entry(req, &e);
			return;
		}
	}
	fuse_reply_err(req, ENOENT);
	printf("File not found: %s\n", name);
	return;
}



// readdir-> lists all files inside of a directory
static void tosfs_readdir(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi)
{
	

	struct tosfs_inode* dir_inode;
	if (get_inode_with_id(&dir_inode, ino)== -1){
		fuse_reply_err(req, ENOENT);
		return;
	};

	if ((dir_inode->mode & S_IFMT) != S_IFDIR){ // checks if the file is a directory
		fuse_reply_err(req, ENOTDIR);
		return;
	}
	
	struct dirbuf b; // structured copied from the hello_ll.c example file
	memset(&b, 0, sizeof(b));

	int block_no = dir_inode->block_no;
	int n_link = dir_inode->nlink;
	struct tosfs_dentry* dentry_table = (struct tosfs_dentry*)(map + TOSFS_BLOCK_SIZE*block_no);
	for(int i = 0; i<n_link; i++){
		dirbuf_add(req, &b, dentry_table[i].name, ino);
	}

	reply_buf_limited(req, b.p, b.size, off, size);
	free(b.p);
}

// open-> checks if the file is indeed a file (and not a directory)
static void tosfs_open(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi)
{
	struct tosfs_inode* current_inode;
	if(get_inode_with_id(&current_inode, ino) == -1){
		fuse_reply_err(req, ENOENT);
		return;
	};
	if ((current_inode->mode & S_IFMT) == S_IFDIR) {
        fuse_reply_err(req, EISDIR);
        return;
    }
    fuse_reply_open(req, fi);
}

// read-> returns bytes read on a specific file
static void tosfs_read(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi)
{
	(void) fi;
	struct tosfs_inode* current_inode;
	if(get_inode_with_id(&current_inode, ino) == -1){
		fuse_reply_err(req, ENOENT);
		return;
	};
	int block_no = current_inode->block_no;
	char* res_str = (char*)(map + block_no*TOSFS_BLOCK_SIZE);
	reply_buf_limited(req, res_str, TOSFS_BLOCK_SIZE, off, size);
}

// create-> creates a file (with its inode and data block)
static void tosfs_create(fuse_req_t req, fuse_ino_t parent, const char *name, mode_t mode, struct fuse_file_info *fi){
	
	if(superblock->inodes==32){ // case where the filesystem is already full
		fuse_reply_err(req, ENOSPC);
		return;
	}
	
	struct tosfs_inode* dir_inode;
	if(get_inode_with_id(&dir_inode, parent) == -1){
		fuse_reply_err(req, ENOENT);
		return;
	};
    if ((dir_inode->mode & S_IFMT) != S_IFDIR) {
        fuse_reply_err(req, ENOTDIR);
        return;
    }

	// setup new inode
    int new_ino = superblock->inodes+1;
    struct tosfs_inode *new_inode = &inodes[new_ino];
    memset(new_inode, 0, sizeof(*new_inode));
	new_inode->mode = (mode & 0777) | S_IFREG;
    new_inode->size = 0;
	new_inode->inode = new_ino;
	new_inode->uid = 0;
	new_inode->gid = 0;
	new_inode->perm = mode & 0777;
	new_inode->nlink = 0;
	new_inode->block_no = superblock->inodes+1;

	int block_no = dir_inode->block_no;
	int n_link = dir_inode->nlink;
	
	dir_inode->nlink++;

	// adding the dentry to the datablock
	struct tosfs_dentry* new_dentry = (struct tosfs_dentry*)(map + block_no*TOSFS_BLOCK_SIZE + n_link*sizeof(struct tosfs_dentry));
	new_dentry->inode=new_ino;
	strcpy(new_dentry->name, name); 

	// updating superblock metadata
	superblock->inodes++;
	tosfs_set_bit(superblock->inode_bitmap,new_ino);
	tosfs_set_bit(superblock->block_bitmap,superblock->inodes);

    struct fuse_entry_param e;
    memset(&e, 0, sizeof(e));
    e.ino = new_ino;
    e.attr_timeout = 1.0;
    e.entry_timeout = 1.0;
    tosfs_stat(e.ino, &e.attr);
    fuse_reply_create(req, &e, fi);
}

// setattr-> sets the values for the inode of a specific file
static void tosfs_setattr(fuse_req_t req, fuse_ino_t ino, struct stat *attr,
                         int to_set, struct fuse_file_info *fi)
{
    struct tosfs_inode* target_inode;
	if (get_inode_with_id(&target_inode, ino) == -1){
		fuse_reply_err(req, ENOENT);
		return;
	};
    
    struct stat stbuf;
    memset(&stbuf, 0, sizeof(stbuf));
    if (tosfs_stat(ino, &stbuf) == 0) {
		fuse_reply_attr(req, &stbuf, 1.0);
    } else {
		fuse_reply_err(req, ENOENT);
    }
}

// mkdir-> creates a directory (with its inode and data block)
static void tosfs_mkdir(fuse_req_t req, fuse_ino_t parent, const char *name, mode_t mode){

	if (superblock->inodes==32){
		fuse_reply_err(req, ENOSPC);
		return;
	}
	struct tosfs_inode* dir_inode;
	if(get_inode_with_id(&dir_inode, parent) == -1){
		fuse_reply_err(req, ENOENT);
		return;
	};

    if ((dir_inode->mode & S_IFMT) != S_IFDIR) {
		fuse_reply_err(req, ENOTDIR);
        return;
    }
    int new_ino = superblock->inodes+1;
    struct tosfs_inode *new_inode = &inodes[new_ino];
    memset(new_inode, 0, sizeof(*new_inode));
	new_inode->mode = (mode & 0777) | S_IFDIR;//	-> only this line differs from tosfs_create
    new_inode->size = 0;
	new_inode->inode = new_ino;
	new_inode->uid = 0;
	new_inode->gid = 0;
	new_inode->perm = mode & 0777;
	new_inode->nlink = 0;
	new_inode->block_no = superblock->inodes+1;
	
	int block_no = dir_inode->block_no;
	int n_link = dir_inode->nlink;
	dir_inode->nlink++;
	
	// adding the dentry to the datablock
	struct tosfs_dentry* new_dentry = (struct tosfs_dentry*)(map + block_no*TOSFS_BLOCK_SIZE + n_link*sizeof(struct tosfs_dentry));
	new_dentry->inode=new_ino;
	strcpy(new_dentry->name, name);

	// updating superblock metadata
	superblock->inodes++;
	tosfs_set_bit(superblock->inode_bitmap,new_ino);
	tosfs_set_bit(superblock->block_bitmap,superblock->inodes);

	printf("Ok\n\r");
	struct fuse_entry_param e;
    memset(&e, 0, sizeof(e));
    e.ino = new_ino;
    e.attr_timeout = 1.0;
    e.entry_timeout = 1.0;
    tosfs_stat(e.ino, &e.attr);
    fuse_reply_entry(req, &e);
}

// write-> writes data in a file
static void tosfs_write(fuse_req_t req, fuse_ino_t ino, const char *buf, size_t size, off_t off, struct fuse_file_info *fi){
	
	if ((size==0) | (size>TOSFS_BLOCK_SIZE)){ 
		fuse_reply_err(req, EINVAL);
		return;
	}

	struct tosfs_inode* current_inode;
	if (get_inode_with_id(&current_inode, ino) == -1){
		fuse_reply_err(req, ENOENT);
		return;
	};
	current_inode->size = size;
	int block_no = current_inode->block_no;
	char* data = (map + block_no*TOSFS_BLOCK_SIZE + off);
	if(memcpy(data, buf, size)==NULL){
		fuse_reply_err(req, EIO);
		return;
	}
	fuse_reply_write(req, size);
}

// fuse operations definitions
static struct fuse_lowlevel_ops oper = {
	.lookup		= tosfs_lookup,
	.getattr	= tosfs_getattr,
	.setattr	= tosfs_setattr,
	.readdir	= tosfs_readdir,
	.open		= tosfs_open,
	.read		= tosfs_read,
	.create		= tosfs_create,
	.mkdir		= tosfs_mkdir,
	.write		= tosfs_write
};



int main(int argc, char* argv[]) {
	// Mapping the filesystem base into the memory
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

	map = mmap(NULL, stats.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (map == MAP_FAILED) {
		perror("Could not map file");
		close(fd);
		return 1;
	}

	// Mapping the filesystem structures
	superblock = (struct tosfs_superblock *)map;
	inodes = (struct tosfs_inode *)(map + TOSFS_INODE_BLOCK * TOSFS_BLOCK_SIZE);
	dentries = (struct tosfs_dentry *)(map + TOSFS_ROOT_BLOCK * TOSFS_BLOCK_SIZE);


	// Setting default values for base files (it seems like the base filesystem does not define these ones)
	superblock->inodes = 3;
	struct tosfs_inode* root_inode;
	if (get_inode_with_id(&root_inode, superblock->root_inode) == -1) {
		printf("ERROR: Root inode not found\n");
		munmap(map, stats.st_size);
		close(fd);
		return 1;
	}

	root_inode->nlink = 4; // root directory has 4 links: ., .., one_file.txt, and two_file.txt

	// launch fuse mounting
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

