/*
 * HUST OS Lab 04 - Directory
 *
 * By Pan Yue
 */

#include "../zxcpyplib/zxcpyp_sys.h"
#include <dirent.h>
#include <locale.h>  /* Set sorting order using system locale sort */
#include <pwd.h>     /* Get username */
#include <grp.h>     /* Get groupname */
#include <time.h>    /* Get last modify time */

#define LS_BLOCK_SIZE 1024

/* Cmp function for quick sort */
int dir_cmp(const void *a, const void *b) {
  struct dirent *dir_a = (struct dirent*)a;
  struct dirent *dir_b = (struct dirent*)b;
  /* Compare for Chinese */
  return strcoll(dir_a->d_name, dir_b->d_name);
}

/* Print status of a file */
void print_status(char *file, int link_num, int size_num) {
  char modstr[11];
  char *time;
  struct stat s_buf;
  struct passwd *pwd_buf;
  struct group *group_buf;
  if (stat(file, &s_buf) != 0)
    err_exit("Stat");
  /* Set mod str */
  strcpy(modstr, "----------");
  /* File type */
  if (S_ISDIR(s_buf.st_mode))
	  modstr[0] = 'd';
	if (S_ISCHR(s_buf.st_mode))
		modstr[0] = 'c';
	if (S_ISBLK(s_buf.st_mode))
		modstr[0] = 'b';
  if (S_ISFIFO(s_buf.st_mode))
    modstr[0] = 'p';
  /* User mod */
	if (s_buf.st_mode & S_IRUSR)
		modstr[1] = 'r';
	if (s_buf.st_mode & S_IWUSR)
		modstr[2] = 'w';
	if (s_buf.st_mode & S_IXUSR)
		modstr[3] = 'x';
  /* Group mod */
	if (s_buf.st_mode & S_IRGRP)
		modstr[4] = 'r';
	if (s_buf.st_mode & S_IWGRP)
		modstr[5] = 'w';
	if (s_buf.st_mode & S_IXGRP)
		modstr[6] = 'x';
  /* Other mod */
	if (s_buf.st_mode & S_IROTH)
		modstr[7] = 'r';
	if (s_buf.st_mode & S_IWOTH)
		modstr[8] = 'w';
	if (s_buf.st_mode & S_IXOTH)
		modstr[9] = 'x';
  /* Get username */
  if ((pwd_buf = getpwuid(s_buf.st_uid)) == NULL)
    err_exit("Username get");
  if ((group_buf = getgrgid(s_buf.st_gid)) == NULL)
    err_exit("Groupname get");
  /* Get last modify time */
  time = ctime(&s_buf.st_mtime);
  /* Print file info */
  printf("%s %*d %s %s %*ld %.12s %-s\n", \
		modstr, link_num, (int)s_buf.st_nlink, pwd_buf->pw_name, group_buf->gr_name, size_num, (long)s_buf.st_size, time + 4, file);
}

/* List all files */
void list_files(char *fullpath, char *path) {
  struct dirent *entry = NULL;
  struct stat s_buf;

  DIR *directory = opendir(path);
  if (directory == NULL)
    err_exit("Open dir");

  /* Change path */
  chdir(path);
  if (strcmp(path, fullpath) != 0)  /* Print '\n' if not first */
    printf("\n");
  printf("%s:\n", fullpath);

  /* Get dirent num */
  int entry_count = 0;  
  while ((entry = readdir(directory)) != NULL) {
    /* Skip . and .. */
    if (strncmp(entry->d_name,  ".", 1) == 0)
      continue;
    entry_count++;
  }

  /* Get dirent struct */
  int pos = 0;
  rewinddir(directory);
  struct dirent *entry_bufs = (struct dirent*)malloc(sizeof(struct dirent) * entry_count);
  while ((entry = readdir(directory)) != NULL) {
    /* Skip . and .. */
    if (strncmp(entry->d_name,  ".", 1) == 0)
      continue;
    memcpy(entry_bufs + pos, entry, sizeof(struct dirent));
    pos++;
  }

  /* Quick sort */
  qsort(entry_bufs, entry_count, sizeof(struct dirent), dir_cmp);

  /* Count total blocks */
  int total = 0;
  int link_max = 0, size_max = 0;
  int link_num = 0, size_num = 0;
  for (pos = 0; pos < entry_count; pos++) {
    entry = entry_bufs + pos;
    stat(entry->d_name, &s_buf);
    /* Get max link num and size num */
    if (s_buf.st_nlink > link_max)
      link_max = s_buf.st_nlink;
    if (s_buf.st_size > size_max)
      size_max = s_buf.st_size;
    /* Add total blocks */
      total += s_buf.st_blocks;
  }
  /* Count num of max link num and max size num */
  for (; link_max; link_max /= 10)
    link_num++;
  for (; size_max; size_max /= 10)
    size_num++;

  printf("total %d\n", total * 512 / LS_BLOCK_SIZE);

  /* List file */
  for (pos = 0; pos < entry_count; pos++) {
    entry = entry_bufs + pos;
    print_status(entry->d_name, link_num, size_num);
  }

  /* Recursive */
  for (int pos = 0; pos < entry_count; pos++) {
    entry = entry_bufs + pos;
    /* Link new path */
    char *new_path = (char*)malloc(sizeof(*fullpath) + 2 + sizeof(entry->d_name));
    strcpy(new_path, fullpath);
    strcat(new_path, "/");
    strcat(new_path, entry->d_name);
    if (entry ->d_type == 4)
      list_files(new_path, entry->d_name);
  }
  chdir("..");
  closedir(directory);
}

int main(int argc, char **argv) {

  /* Check args */
  if (argc != 2) {
    usage_err("./directory <path>");
    exit(1);
  }

  /* Set sorting order using system locale sort
   * LC_COLLATE: local sort order
   */
  setlocale(LC_COLLATE, "");

  /* Get path of the file */
  struct stat s_buf;
  char *path = (char *)malloc(sizeof(argv[1]) + 1);
  strcpy(path, argv[1]);

  /* Judeg if this is a dir */
  stat(path, &s_buf);
  if (S_ISDIR(s_buf.st_mode))
    list_files(path, path);
  else {
    int link_num = 0;
    int size_num = 0;
    for (; s_buf.st_nlink; s_buf.st_nlink /= 10)
      link_num++;
    for (; s_buf.st_size; s_buf.st_size /= 10)
      size_num++;
    print_status(path, link_num, size_num);
  }
  return 0;
}
