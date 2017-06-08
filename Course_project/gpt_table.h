#ifndef GPT_TABLE_H
#define GPT_TABLE_H

#include "other.h"

int gpt_read_table(struct device *);
void gpt_show_table(struct device *);
void gpt_check_free_space(struct device *);
void gpt_show_free_space(struct device *);
int gpt_delete_partition(struct device *, int);

#endif // GPT_TABLE_H
