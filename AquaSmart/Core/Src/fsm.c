/*
 * fsm.c
 *
 *  Created on: Mar 28, 2020
 *      Author: villa
 */

#include "fsm.h"
#include <stdlib.h>

fsm_t* fsm_new (fsm_trans_t* tt)
{
  fsm_t* this = (fsm_t*) malloc (sizeof (fsm_t));
  fsm_init (this, tt);
  return this;
}

void fsm_init (fsm_t* this, fsm_trans_t* tt)
{
  this->tt = tt;
  this->current_state = tt->orig_state;
}

void fsm_fire (fsm_t* this)
{
  fsm_trans_t* t;
  for (t = this->tt; t->orig_state >= 0; ++t) {
    if ((this->current_state == t->orig_state) && t->in(this)) {
      this->current_state = t->dest_state;
      if (t->out)
        t->out(this);
      break;
    }
  }
}
