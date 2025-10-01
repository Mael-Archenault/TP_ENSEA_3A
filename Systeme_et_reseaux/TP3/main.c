#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "tosfs.h"


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


	return 0;
}