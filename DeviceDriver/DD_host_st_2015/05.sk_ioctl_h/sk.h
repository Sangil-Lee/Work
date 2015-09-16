#ifndef __SK_H__
#define	__SK_H__

#define	SK_MAGIC	'K'
#define	SK_MAXNR	6

typedef struct {
	unsigned int size;
	char buff[128];
} __attribute__((packed))SK_INFO;

//remove CPU padding data

#define	SK_LED_OFF	_IO(SK_MAGIC, 0)
#define	SK_LED_ON	_IO(SK_MAGIC, 1)
#define	SK_GETSTATE	_IO(SK_MAGIC, 2)

#define	SK_READ		_IOR(SK_MAGIC, 3, SK_INFO)
#define	SK_WRITE	_IOW(SK_MAGIC, 4, SK_INFO)
#define	SK_RW		_IOWR(SK_MAGIC, 5, SK_INFO)

#endif
