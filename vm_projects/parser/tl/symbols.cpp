#include "symbols.h"

namespace tl
{

bool symbols::_inited = false;

std::array<symbols::item, symbols::_MAX_SYMBOLS> symbols::_items = {
    // must remain sorted because we do a binary search
    item{ "-",        SUB      },
    item{ "*",        MUL      },
    item{ "/",        DIV      },
    item{ "+",        ADD      },
    item{ "append",   APPEND   },
    item{ "apply",    APPLY    },
    item{ "atom?",    ATOMP    },
    item{ "cons",     CONS     },
    item{ "cons?",    CONSP    },
    item{ "def",      DEF      },
    item{ "define",   DEFINE   },
    item{ "end?",     ENDP     },
    item{ "first",    FIRST    },
    item{ "function", FUNCTION },
    item{ "head",     HEAD     },
    item{ "lambda",   LAMBDA   },
    item{ "len",      LEN      },
    item{ "length",   LENGTH   },
    item{ "let",      LET      },
    item{ "list",     LIST     },
    item{ "list?",    LISTP    },
    item{ "map",      MAP      },
    item{ "nil",      NIL      },
    item{ "nil?",     NILP     },
    item{ "number?",  NUMBERP  },
    item{ "pair",     PAIR     },
    item{ "pair?",    PAIRP    },
    item{ "quote",    QUOTE    },
    item{ "rest",     REST     },
    item{ "set!",     SETF     },
    item{ "set?",     SETP     },
    item{ "symbol?",  SYMBOLP  },
    item{ "t",        T        },
    item{ "tail",     TAIL     },
};

}
