/*
 * Event receiver serial link
 */

#ifndef _EVR_H_
#define _EVR_H_

void evrInit(void);
void evrShowStatus(void);
void evrGetTime(uint32_t *seconds, uint32_t *ticks);

#endif /* _EVR_H_ */
