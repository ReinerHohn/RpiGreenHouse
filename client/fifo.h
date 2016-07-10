#ifndef FIFO_H_
#define FIFO_H_
#define CON_OUT_BUF_SIZE           64

/* Buffer masks */
#define CON_OUT_MASK               (CON_OUT_BUF_SIZE-1ul)

/* Buffer read / write macros */
#define CON_BUF_RESET(Fifo)      (Fifo->rd_idx = Fifo->wr_idx = 0)
#define CON_BUF_WR(Fifo, dataIn) (Fifo->data[CON_OUT_MASK & Fifo->wr_idx++] = (dataIn))
#define CON_BUF_RD(Fifo)         (Fifo->data[CON_OUT_MASK & Fifo->rd_idx++])
#define CON_BUF_EMPTY(Fifo)      ((CON_OUT_MASK & Fifo->rd_idx) == (CON_OUT_MASK & Fifo->wr_idx))
#define CON_BUF_FULL(Fifo)       ((CON_OUT_MASK & Fifo->rd_idx) == (CON_OUT_MASK & Fifo->wr_idx+1))
#define CON_BUF_COUNT(Fifo)      ((CON_OUT_MASK & Fifo.wr_idx) - (CON_OUT_MASK & Fifo.rd_idx))

// Serial output buffer
typedef struct {
  unsigned char data[CON_OUT_BUF_SIZE];
  unsigned int wr_idx;
  unsigned int rd_idx;
} CON_Fifo_t;


#endif //FIFO_H_
