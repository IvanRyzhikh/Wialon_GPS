#pragma once

#include <cerrno>
#include <cstdint>
#include <expected>

namespace law {
    enum class RetCode : std::uint16_t {
        SUCCESS = 0,

        X_FIRST_ERR = 1,
        X_EPERM = EPERM,
        X_ENOENT = ENOENT,
        X_ESRCH = ESRCH,
        X_EINTR = EINTR,
        X_EIO = EIO,
        X_ENXIO = ENXIO,
        X_E2BIG = E2BIG,
        X_ENOEXEC = ENOEXEC,
        X_EBADF = EBADF,
        X_ECHILD = ECHILD,
        X_EAGAIN = EAGAIN,
        X_EWOULDBLOCK = X_EAGAIN,
        X_ENOMEM = ENOMEM,
        X_EACCES = EACCES,
        X_EFAULT = EFAULT,
        X_ENOTBLK = ENOTBLK,
        X_EBUSY = EBUSY,
        X_EEXIST = EEXIST,
        X_EXDEV = EXDEV,
        X_ENODEV = ENODEV,
        X_ENOTDIR = ENOTDIR,
        X_EISDIR = EISDIR,
        X_EINVAL = EINVAL,
        X_ENFILE = ENFILE,
        X_EMFILE = EMFILE,
        X_ENOTTY = ENOTTY,
        X_ETXTBSY = ETXTBSY,
        X_EFBIG = EFBIG,
        X_ENOSPC = ENOSPC,
        X_ESPIPE = ESPIPE,
        X_EROFS = EROFS,
        X_EMLINK = EMLINK,
        X_EPIPE = EPIPE,
        X_EDOM = EDOM,
        X_ERANGE = ERANGE,
        X_EDEADLK = EDEADLK,
        X_ENAMETOOLONG = ENAMETOOLONG,
        X_ENOLCK = ENOLCK,
        X_ENOSYS = ENOSYS,
        X_ENOTEMPTY = ENOTEMPTY,
        X_ELOOP = ELOOP,
        X_ENOMSG = ENOMSG,
        X_EIDRM = EIDRM,
        X_ECHRNG = ECHRNG,
        X_EL2NSYNC = EL2NSYNC,
        X_EL3HLT = EL3HLT,
        X_EL3RST = EL3RST,
        X_ELNRNG = ELNRNG,
        X_EUNATCH = EUNATCH,
        X_ENOCSI = ENOCSI,
        X_EL2HLT = EL2HLT,
        X_EBADE = EBADE,
        X_EBADR = EBADR,
        X_EXFULL = EXFULL,
        X_ENOANO = ENOANO,
        X_EBADRQC = EBADRQC,
        X_EBADSLT = EBADSLT,
        X_EBFONT = EBFONT,
        X_ENOSTR = ENOSTR,
        X_ENODATA = ENODATA,
        X_ETIME = ETIME,
        X_ENOSR = ENOSR,
        X_ENONET = ENONET,
        X_ENOPKG = ENOPKG,
        X_EREMOTE = EREMOTE,
        X_ENOLINK = ENOLINK,
        X_EADV = EADV,
        X_ESRMNT = ESRMNT,
        X_ECOMM = ECOMM,
        X_EPROTO = EPROTO,
        X_EMULTIHOP = EMULTIHOP,
        X_EDOTDOT = EDOTDOT,
        X_EBADMSG = EBADMSG,
        X_EOVERFLOW = EOVERFLOW,
        X_ENOTUNIQ = ENOTUNIQ,
        X_EBADFD = EBADFD,
        X_EREMCHG = EREMCHG,
        X_ELIBACC = ELIBACC,
        X_ELIBBAD = ELIBBAD,
        X_ELIBSCN = ELIBSCN,
        X_ELIBMAX = ELIBMAX,
        X_ELIBEXEC = ELIBEXEC,
        X_EILSEQ = EILSEQ,
        X_ERESTART = ERESTART,
        X_ESTRPIPE = ESTRPIPE,
        X_EUSERS = EUSERS,
        X_ENOTSOCK = ENOTSOCK,
        X_EDESTADDRREQ = EDESTADDRREQ,
        X_EMSGSIZE = EMSGSIZE,
        X_EPROTOTYPE = EPROTOTYPE,
        X_ENOPROTOOPT = ENOPROTOOPT,
        X_EPROTONOSUPPORT = EPROTONOSUPPORT,
        X_ESOCKTNOSUPPORT = ESOCKTNOSUPPORT,
        X_EOPNOTSUPP = EOPNOTSUPP,
        X_EPFNOSUPPORT = EPFNOSUPPORT,
        X_EAFNOSUPPORT = EAFNOSUPPORT,
        X_EADDRINUSE = EADDRINUSE,
        X_EADDRNOTAVAIL = EADDRNOTAVAIL,
        X_ENETDOWN = ENETDOWN,
        X_ENETUNREACH = ENETUNREACH,
        X_ENETRESET = ENETRESET,
        X_ECONNABORTED = ECONNABORTED,
        X_ECONNRESET = ECONNRESET,
        X_ENOBUFS = ENOBUFS,
        X_EISCONN = EISCONN,
        X_ENOTCONN = ENOTCONN,
        X_ESHUTDOWN = ESHUTDOWN,
        X_ETOOMANYREFS = ETOOMANYREFS,
        X_ETIMEDOUT = ETIMEDOUT,
        X_ECONNREFUSED = ECONNREFUSED,
        X_EHOSTDOWN = EHOSTDOWN,
        X_EHOSTUNREACH = EHOSTUNREACH,
        X_EALREADY = EALREADY,
        X_EINPROGRESS = EINPROGRESS,
        X_ESTALE = ESTALE,
        X_EUCLEAN = EUCLEAN,
        X_ENOTNAM = ENOTNAM,
        X_ENAVAIL = ENAVAIL,
        X_EISNAM = EISNAM,
        X_EREMOTEIO = EREMOTEIO,
        X_EDQUOT = EDQUOT,
        X_ENOMEDIUM = ENOMEDIUM,
        X_EMEDIUMTYPE = EMEDIUMTYPE,
        X_ECANCELED = ECANCELED,
        X_ENOKEY = ENOKEY,
        X_EKEYEXPIRED = EKEYEXPIRED,
        X_EKEYREVOKED = EKEYREVOKED,
        X_EKEYREJECTED = EKEYREJECTED,
        X_EOWNERDEAD = EOWNERDEAD,
        X_ENOTRECOVERABLE = ENOTRECOVERABLE,
        X_LAST_ERR = X_ENOTRECOVERABLE,

        LAW_FIRST_ERR = 1000,
        NOT_IMPLEMENTED = LAW_FIRST_ERR, /* This functionality not implemented yet */
        NOT_SUPPORTED = LAW_FIRST_ERR + 1, /* This functionality not supported for this object */
        INCORRECT_HOSTNAME = LAW_FIRST_ERR + 2, /* Can't found at least one ip attached to specified hostname */
        INVALID_HOSTNAME_RESOLUTION = LAW_FIRST_ERR + 3, /* Resolution of hostname failed */
        INCORRECT_IP = LAW_FIRST_ERR + 4, /* Specified IP address is incorrect */
        TIMEOUT = LAW_FIRST_ERR + 5, /* Operation timeout reached */
        READ_EMPTY_DATA = LAW_FIRST_ERR + 6, /* Read data is empty */
        INVALID_FILE_DESCRIPTOR = LAW_FIRST_ERR + 7, /* File descriptor is invalid */
        NO_AVAILABLE_SPACE = LAW_FIRST_ERR + 8, /* Max capacity of internal buffer reached */
        INVALID_ARGUMENT = LAW_FIRST_ERR + 9,
        LAW_LAST_ERR = INVALID_ARGUMENT,
    };

    using Status = std::expected<void, RetCode>;

    template <typename T>
    using CustomError = std::expected<T, RetCode>;

    template <typename T = void>
    static CustomError<T> fromErrno() noexcept {
        return std::unexpected(RetCode(errno));
    }

    template <typename T = void>
    static CustomError<T> fromLawError(RetCode rc) noexcept {
        return std::unexpected(rc);
    }

    void throwErrno();

    void throwLawError(RetCode rc);

    static constexpr bool isErrno(RetCode rc) {
        return (rc >= RetCode::X_FIRST_ERR && rc <= RetCode::X_LAST_ERR);
    }
}
