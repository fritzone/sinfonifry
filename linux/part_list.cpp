#include <stdlib.h>
#include <blkid.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <ext2fs/ext2fs.h>
#include <unistd.h>

#define OUTPUT_VALUE_ONLY       0x0001
#define OUTPUT_DEVICE_ONLY      0x0002
#define OUTPUT_PRETTY_LIST      0x0004

static void safe_print(const char *cp, int len)
{
  unsigned char   ch;

  if (len < 0)
    len = strlen(cp);

  while (len--) {
    ch = *cp++;
    if (ch > 128) {
      fputs("M-", stdout);
      ch -= 128;
    }
    if ((ch < 32) || (ch == 0x7f)) {
      fputc('^', stdout);
      ch ^= 0x40; /* ^@, ^A, ^B; ^? for DEL */
    }
    fputc(ch, stdout);
  }
}

static int get_terminal_width()
{
  return 80;
}

static int pretty_print_word(const char *str, int max_len,
                             int left_len, int overflow_nl)
{
  int len = strlen(str) + left_len;
  int ret = 0;

  fputs(str, stdout);
  if (overflow_nl && len > max_len) {
    fputc('\n', stdout);
    len = 0;
  } else if (len > max_len)
    ret = len - max_len;
  do
    fputc(' ', stdout);
  while (len++ < max_len);
  return ret;
}

static void pretty_print_line(const char *device, const char *fs_type,
                              const char *label, const char *mtpt,
                              const char *uuid)
{
  static int device_len = 10, fs_type_len = 7;
  static int label_len = 8, mtpt_len = 14;
  static int term_width = -1;
  int len, w;

  if (term_width < 0)
    term_width = get_terminal_width();

  if (term_width > 80) {
    term_width -= 80;
    w = term_width / 10;
    if (w > 8)
      w = 8;
    term_width -= 2*w;
    label_len += w;
    fs_type_len += w;
    w = term_width/2;
    device_len += w;
    mtpt_len +=w;
  }

  len = pretty_print_word(device, device_len, 0, 1);
  len = pretty_print_word(fs_type, fs_type_len, len, 0);
  len = pretty_print_word(label, label_len, len, 0);
  len = pretty_print_word(mtpt, mtpt_len, len, 0);
  fputs(uuid, stdout);
  fputc('\n', stdout);
}

static void pretty_print_dev(blkid_dev dev)
{
  blkid_tag_iterate       iter;
  const char              *type, *value, *devname;
  const char              *uuid = "", *fs_type = "", *label = "";
  int                     len, mount_flags;
  char                    mtpt[80];
  errcode_t               retval;

  if (dev == NULL) {
    pretty_print_line("device", "fs_type", "label",
		      "mount point", "UUID");
    for (len=get_terminal_width()-1; len > 0; len--)
      fputc('-', stdout);
    fputc('\n', stdout);
    return;
  }

  devname = blkid_dev_devname(dev);
  if (access(devname, F_OK))
    return;

  /* Get the uuid, label, type */
  iter = blkid_tag_iterate_begin(dev);
  while (blkid_tag_next(iter, &type, &value) == 0) {
 printf( "-->%s<--\n", type);

    if (!strcmp(type, "UUID"))
      uuid = value;
    if (!strcmp(type, "TYPE"))
      fs_type = value;
    if (!strcmp(type, "LABEL"))
      label = value;
  }
  blkid_tag_iterate_end(iter);

  /* Get the mount point */
  mtpt[0] = 0;
  retval = ext2fs_check_mount_point(devname, &mount_flags,
				    mtpt, sizeof(mtpt));
  if (retval == 0) {
    if (mount_flags & EXT2_MF_MOUNTED) {
      if (!mtpt[0])
	strcpy(mtpt, "(mounted, mtpt unknown)");
    } else if (mount_flags & EXT2_MF_BUSY)
      strcpy(mtpt, "(in use)");
    else
      strcpy(mtpt, "(not mounted)");
  }

  pretty_print_line(devname, fs_type, label, mtpt, uuid);
}

int main()
{
  blkid_dev_iterate       iter;
  blkid_dev               dev;
  blkid_cache             cache = NULL;
  char *search_type = NULL, *search_value = NULL;
  char *read = NULL;

  blkid_get_cache(&cache, read);

  blkid_probe_all(cache);

  iter = blkid_dev_iterate_begin(cache);
  blkid_dev_set_search(iter, search_type, search_value);
  while (blkid_dev_next(iter, &dev) == 0) 
  {
    dev = blkid_verify(cache, dev);
    if (!dev)
      continue;
    pretty_print_dev(dev);
  }
  blkid_dev_iterate_end(iter);

}
