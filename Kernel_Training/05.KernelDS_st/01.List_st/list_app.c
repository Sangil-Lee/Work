#include <stdio.h>
#include <stdlib.h>

#define	offsetof(type, member)	((unsigned int)(&((type *)0)->member))

#define	container_of(ptr, type, member)	({\
	const typeof(((type*)0)->member) *_mptr=(ptr); \
	(type*)((char*)_mptr - offsetof(type, member)); })

struct list_head {
	struct list_head *next;
	struct list_head *prev;
};

#define	LIST_HEAD_INIT(name)	{&(name), &(name)}

#define	LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

#define	list_entry(ptr, type, member) \
	........ (ptr, type, member)

#define	.......... (pos, head) \
	for(pos=(head)->next; pos!=(head); pos=pos->next)


static inline void __list_add(struct list_head *new,
	struct list_head *prev, struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

struct info {
	int type;
	int color;
	struct list_head list;
};

struct info goods1 = {
	.type = 1,
	.color = 3,
	.list = ......... (goods1.list)
};
struct info goods2 = {
	.type = 2,
	.color = 1,
	.list = LIST_HEAD_INIT(goods2.list)
};
struct info goods3 = {
	.type = 3,
	.color = 2,
	.list = LIST_HEAD_INIT(goods2.list)
};

static LIST_HEAD(goods_list);

int main(void)
{
	struct list_head *lp;
	struct info *gp;

	printf("size => %d, %d, %d\n", 
		sizeof(struct list_head), sizeof(int), sizeof(struct info));
	printf("address => %p, %p, %p, %p\n", 
		&goods1, &goods1.type, &goods1.color, &goods1.list);
	printf("offset => %u, %u, %u\n",
		offsetof(struct info, type),
		offsetof(struct info, color),
		offsetof(struct info, list));
	printf("container_of => %p, %p\n",
		container_of((int *)&goods1.type, struct info, type),
		container_of((struct list_head *)&goods1.list, struct info, list));


	list_add(&goods1.list, &goods_list);
	list_add(&goods2.list, &goods_list);
	list_add(&goods3.list, &goods_list);

	gp = malloc(sizeof(*gp));
	gp->type = 2;
	gp->color = 2;
	list_add_tail(&gp->list, &goods_list);

	list_for_each(lp, &goods_list) {
		gp = list_entry(lp, struct info, list);
		printf("goods : %d, %d\n", gp->type, gp->color);
	}

	return 0;
}

