#include "cheat.h"

int cheat_clear_flags(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    cheat_clearflag_constant(prCheat);
    cheat_clearflag_fresh(prCheat);
    cheat_clearflag_selected(prCheat);
    return CHEAT_SUCCESS;
}

int cheat_clearflag_constant(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    prCheat->flags &= ~CHEAT_FLAG_CONSTANT;
    return CHEAT_SUCCESS;
}

int cheat_clearflag_fresh(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    prCheat->flags &= ~CHEAT_FLAG_FRESH;
    return CHEAT_SUCCESS;
}

int cheat_clearflag_selected(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    prCheat->flags &= ~CHEAT_FLAG_SELECTED;
    return CHEAT_SUCCESS;
}

unsigned int cheat_get_block(Cheat* prCheat) {
    unsigned int block = 0;
    if (prCheat != NULL) {
        block = (unsigned int)prCheat->block;
    }
    return block;
}

unsigned int cheat_get_length(Cheat *prCheat) {
    unsigned int clen = 0;
    if (prCheat != NULL) {
        clen = (unsigned int)prCheat->len;
    }
    return clen;
}

int cheat_init(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_NULLPTR;
    }
    prCheat->block = (unsigned short) 0;
    prCheat->flags = (unsigned char) 0;
    prCheat->len = (unsigned short) 0;
    prCheat->name[0] = (char) 0;
    return CHEAT_SUCCESS;
}

int cheat_is_constant(Cheat* prCheat) {
    int is = 0;
    if (prCheat != NULL) {
        is = (prCheat->flags & CHEAT_FLAG_CONSTANT) ? 1 : 0;
    }
    return is;
}

int cheat_is_fresh(Cheat* prCheat) {
    int is = 0;
    if (prCheat != NULL) {
        is = (prCheat->flags & CHEAT_FLAG_FRESH) ? 1 : 0;
    }
    return is;
}

int cheat_is_inactive(Cheat* prCheat) {
    int is = 0;
    if (prCheat != NULL) {
        if (cheat_is_constant(prCheat) == 0 &&
                cheat_is_selected(prCheat) == 0) {
            is = 1;
        }
    }
    return is;
}

int cheat_is_selected(Cheat* prCheat) {
    int is = 0;
    if (prCheat != NULL) {
        is = (prCheat->flags & CHEAT_FLAG_SELECTED) ? 1 : 0;
    }
    return is;
}

int cheat_set_block(Cheat* prCheat, unsigned int block) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    prCheat->block = (unsigned short)(block & 0xFFFF);
    return CHEAT_SUCCESS;
}

int cheat_set_length(Cheat* prCheat, unsigned int length) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    prCheat->len = (unsigned short)(length & 0xFFFF);
    return CHEAT_SUCCESS;
}

int cheat_setflag_constant(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    prCheat->flags |= CHEAT_FLAG_CONSTANT;
    return CHEAT_SUCCESS;
}

int cheat_setflag_fresh(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    prCheat->flags |= CHEAT_FLAG_FRESH;
    return CHEAT_SUCCESS;
}

int cheat_setflag_selected(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    prCheat->flags |= CHEAT_FLAG_SELECTED;
    return CHEAT_SUCCESS;
}

int cheat_set_constant(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    cheat_clearflag_selected(prCheat);
    cheat_setflag_constant(prCheat);
    cheat_clearflag_fresh(prCheat);
    return CHEAT_SUCCESS;
}

int cheat_set_inactive(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    cheat_clearflag_constant(prCheat);
    cheat_clearflag_selected(prCheat);
    cheat_clearflag_fresh(prCheat);
    return CHEAT_SUCCESS;
}

int cheat_set_selected(Cheat* prCheat) {
    if (prCheat == NULL) {
        return CHEAT_MEMORY;
    }
    cheat_clearflag_constant(prCheat);
    cheat_setflag_selected(prCheat);
    cheat_clearflag_fresh(prCheat);
    return CHEAT_SUCCESS;
}
