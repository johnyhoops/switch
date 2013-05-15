    struct termios tios, old_tios;
    speed_t speed;
    int flags;
    
    flags = O_RDWR | O_NOCTTY | O_NDELAY | O_EXCL;
    err = open(ctx_rtu->device, flags);
    if (err == -1) {
        fprintf(stderr, "ERROR Can't open the device %s (%s)\n",
                ctx_rtu->device, strerror(errno));
        return -1;
    }

    /* Save */
    tcgetattr(ctx->s, &(old_tios));

    memset(&tios, 0, sizeof(struct termios));



    /* Set the baud rate */
    if ((cfsetispeed(&tios, B19200) < 0) ||
        (cfsetospeed(&tios, B19200) < 0)) {
        close(ctx->s);
        ctx->s = -1;
        return -1;
    }


    tios.c_cflag |= (CREAD | CLOCAL);
    tios.c_cflag &= ~CSIZE;
    tios.c_cflag |= CS8;
    tios.c_cflag &=~ CSTOPB;
    tios.c_cflag &=~ PARENB;
    tios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tios.c_iflag &= ~INPCK;
    tios.c_iflag &= ~(IXON | IXOFF | IXANY);
    tios.c_oflag &=~ OPOST;
    tios.c_cc[VMIN] = 0;
    tios.c_cc[VTIME] = 0;

    if (tcsetattr(ctx->s, TCSANOW, &tios) < 0) {
        close(ctx->s);
        ctx->s = -1;
        return -1;
    }
#endif

    return 0;
}
