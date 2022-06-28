#include <stdlib.h>
#include <assert.h>

typedef struct TNode
{
    struct TNode * m_Next;
    char           m_Digit;
} TNODE;

TNODE * createNode ( char digit, TNODE * next )
{
    TNODE * n = (TNODE *) malloc (sizeof ( *n ));
    n -> m_Digit = digit;
    n -> m_Next = next;
    return n;
}

void deleteList (TNODE * l)
{
    while (l)
    {
        TNODE * tmp = l -> m_Next;
        free (l);
        l = tmp;
    }
}

int toHex(char digit)
{
    if((digit - '0') < 10) return digit - '0';
    return digit - 'a' + 10;
}

char toChar(int shiftedVal)
{
    if(shiftedVal < 10) return (char)(shiftedVal + '0');
    return (char)(shiftedVal + 'a' - 10);
}

int isValid(TNODE * a)
{
    while (a){
        if(!(((a->m_Digit >= '0')&&(a->m_Digit <= '9'))||((a->m_Digit >= 'a')&&(a->m_Digit <= 'f')))) return 0;
        if(a->m_Next == NULL && a->m_Digit == '0') return 0;
        a = a->m_Next;
    }
    return 1;
}

TNODE * shiftLeft ( TNODE * a, unsigned int shift )
{
    if(a == NULL) return NULL;
    if(a->m_Digit == '0' && a->m_Next == NULL) return a;
    if(!isValid(a)) return NULL;
    if(shift == 0) return a;
    int carry = 0,add = 0;
    TNODE * head = a,* tail = NULL,* zeroHead = NULL,* zeroTail = NULL;
    unsigned int fourZeros = shift / 4;
    for(unsigned int j = 0;j < fourZeros;++j){
        if(head == a){
            head = tail = createNode ( '0',NULL );
        }
        else{
            tail->m_Next = createNode ( '0',NULL );
            tail = tail->m_Next;
        }
        if(j + 1 == fourZeros){
            zeroTail = tail;
            zeroHead = head;
            head = a;
        }
    }
    unsigned int bits = shift % 4;
    for(unsigned int i = 0; i < bits;++i) {
        for (TNODE *current = head; current != NULL; current = current->m_Next) {
            if (carry == 1) add = 1;
            else add = 0;
            // ((toHex(current->m_Digit) << 1) / 16) != 0
            if ((toHex(current->m_Digit) << 1) / 16) carry = 1;
            else carry = 0;
            if (head == a) {
                tail = head = createNode(toChar((add + (toHex(current->m_Digit) << 1)) % 16), NULL);
            }
            else if(i == 0){
                tail->m_Next = createNode(toChar((add + (toHex(current->m_Digit) << 1)) % 16), NULL);
                tail = tail->m_Next;
            }
            else{
                current->m_Digit = toChar((add + (toHex(current->m_Digit) << 1)) % 16);
            }
        }
        if(carry){
            tail->m_Next = createNode('1', NULL);
            tail = tail->m_Next;
            carry = 0;
        }
    }
    if(zeroHead != NULL){
        zeroTail->m_Next = head;
        return zeroHead;
    }
    return head;
}

int main (int argc, char * argv [])
{
    TNODE * a = NULL, * res = NULL;

    a = createNode ( '3',
                     createNode ( '2',
                                  createNode ( '1', NULL )));
    res = shiftLeft ( a, 1 );
    assert ( res -> m_Digit == '6' );
    assert ( res -> m_Next -> m_Digit == '4' );
    assert ( res -> m_Next -> m_Next -> m_Digit == '2' );
    assert ( res -> m_Next -> m_Next -> m_Next == NULL );
    deleteList ( a );
    deleteList ( res );

    a = createNode ( 'f',
                     createNode ( 'a',
                                  createNode ( '1', NULL )));
    res = shiftLeft ( a, 3 );
    assert ( res -> m_Digit == '8' );
    assert ( res -> m_Next -> m_Digit == '7' );
    assert ( res -> m_Next -> m_Next -> m_Digit == 'd' );
    assert ( res -> m_Next -> m_Next -> m_Next == NULL );
    deleteList ( a );
    deleteList ( res );

    a = createNode ( 'c',
                     createNode ( '5',
                                  createNode ( '4',
                                               createNode ( 'd',
                                                            createNode ( '1', NULL )))));
    res = shiftLeft ( a, 5 );
    assert ( res -> m_Digit == '0' );
    assert ( res -> m_Next -> m_Digit == '8' );
    assert ( res -> m_Next -> m_Next -> m_Digit == 'b' );
    assert ( res -> m_Next -> m_Next -> m_Next -> m_Digit == '8' );
    assert ( res -> m_Next -> m_Next -> m_Next -> m_Next -> m_Digit == 'a' );
    assert ( res -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Digit == '3' );
    assert ( res -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    deleteList ( a );
    deleteList ( res );

    a = createNode ( 'a',
                     createNode ( 'w',
                                  createNode ( '0', NULL )));
    res = shiftLeft ( a, 12 );
    assert ( res == NULL );
    deleteList ( a );

    a = NULL;
    res = shiftLeft ( a, 1 );
    assert ( res == NULL );

    return 0;
}