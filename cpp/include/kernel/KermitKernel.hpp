/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : KermitKernel
 * @created     : Saturday Aug 24, 2019 16:27:47 MDT
 */

#ifndef KERMITKERNEL_HPP

#define KERMITKERNEL_HPP

class KermitKernel {
public:
  KermitKernel();
  virtual ~KermitKernel();

private:
  typedef struct {
    int fd;
    char *port;
    int baud;
  } serial;
};

#endif /* end of include guard KERMITKERNEL_HPP */
