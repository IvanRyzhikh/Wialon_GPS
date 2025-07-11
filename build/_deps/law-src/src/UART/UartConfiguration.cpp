#include <cstring>
#include <stdexcept>
#include <boost/assert.hpp>

#include "law/UART/UartConfiguration.h"

namespace law::uart {
    UartConfiguration::UartConfiguration() noexcept {
        std::memset(&term, 0, sizeof(term));

        term.c_cflag |= (CREAD | CLOCAL | CBAUDEX);
        term.c_cc[VMIN] = 1;
        term.c_cc[VTIME] = 0;
    }

    UartConfiguration &UartConfiguration::setDataBits(DataBits bits) noexcept {
        switch (bits) {
            case DataBits::BITS_5:
                term.c_cflag |= CS5;
                break;
            case DataBits::BITS_6:
                term.c_cflag |= CS6;
                break;
            case DataBits::BITS_7:
                term.c_cflag |= CS7;
                break;
            [[likely]] case DataBits::BITS_8:
                term.c_cflag |= CS8;
                break;
            default:
                __builtin_unreachable();
                break;
        }

        return *this;
    }

    UartConfiguration &UartConfiguration::setParity(Parity par) noexcept {
        switch (par) {
            [[likely]] case Parity::NONE:
                term.c_cflag &= ~(PARENB | PARODD);
                break;
            case Parity::EVEN:
                term.c_cflag |= PARENB;
                term.c_cflag &= ~PARODD;
                break;
            case Parity::ODD:
                term.c_cflag |= PARENB;
                term.c_cflag |= PARODD;
                break;
            default:
                __builtin_unreachable();
                break;
        }

        return *this;
    }

    UartConfiguration &UartConfiguration::setStopBits(StopBits bits) noexcept {
        switch (bits) {
            [[likely]] case StopBits::BITS_1:
                term.c_cflag &= ~CSTOPB;
                break;
            case StopBits::BITS_2:
                term.c_cflag |= CSTOPB;
                break;
            default:
                __builtin_unreachable();
                break;
        }

        return *this;
    }

    UartConfiguration &UartConfiguration::setFlowControl(FlowControl control) noexcept{
        switch (control) {
            [[likely]] case FlowControl::DISABLE:
                term.c_cflag &= ~CRTSCTS;
                break;
            case FlowControl::ENABLE:
                term.c_cflag |= CRTSCTS;
                break;
            default:
                __builtin_unreachable();
                break;
        }

        return *this;
    }

    UartConfiguration &UartConfiguration::setBaudRate(std::uint32_t baud) noexcept {
        term.c_ispeed = baud;
        term.c_ospeed = baud;
        return *this;
    }

    DataBits UartConfiguration::getDataBits() const noexcept {
        switch (term.c_cflag & CSIZE) {
            case CS5:
                return DataBits::BITS_5;
            case CS6:
                return DataBits::BITS_6;
            case CS7:
                return DataBits::BITS_7;
            [[likely]] case CS8:
                return DataBits::BITS_8;
            default:
                __builtin_unreachable();
                break;
        }
    }

    Parity UartConfiguration::getParity() const noexcept {
        switch (term.c_cflag & (PARENB | PARODD)) {
            [[likely]] case 0:
                return Parity::NONE;
            case (PARENB & ~(PARODD)):
                return Parity::EVEN;
            case (PARENB | PARODD):
                return Parity::ODD;
            default:
                __builtin_unreachable();
                break;
        }
    }

    StopBits UartConfiguration::getStopBits(StopBits) const noexcept {
        switch (term.c_cflag & CSTOPB) {
            [[likely]] case 0:
                return StopBits::BITS_1;
            case CSTOPB:
                return StopBits::BITS_2;
            default:
                __builtin_unreachable();
                break;
        }
    }

    FlowControl UartConfiguration::getFlowControl(FlowControl) const noexcept {
        switch (term.c_cflag & CRTSCTS) {
            [[likely]] case 0:
                return FlowControl::DISABLE;
            case CRTSCTS:
                return FlowControl::ENABLE;
            default:
                __builtin_unreachable();
                break;
        }
    }

    std::uint32_t UartConfiguration::getBaudrate() const noexcept {
        BOOST_ASSERT_MSG(term.c_ispeed == term.c_ospeed, "input speed and output speed should be the same (by design)");
        return term.c_ispeed;
    }

    const termios2 &UartConfiguration::getConfiguration() const noexcept {
        return term;
    }
}
