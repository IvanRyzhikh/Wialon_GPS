#ifndef _UBUS_COMMON_H_
#define _UBUS_COMMON_H_

#include <ubox/blobmsg.h>

class BlobBufBuilder {
    private:
        inline static blob_buf bb;
    
    public:
        BlobBufBuilder() {
            blob_buf_init(&bb, 0);
        }
    
        BlobBufBuilder &add(std::string_view name, std::uint8_t data) {
            blobmsg_add_u8(&bb, name.data(), data);
            return *this;
        }

        BlobBufBuilder &add(std::string_view name, std::uint16_t data) {
            blobmsg_add_u16(&bb, name.data(), data);
            return *this;
        }
            
        BlobBufBuilder &add(std::string_view name, std::uint32_t data) {
            blobmsg_add_u32(&bb, name.data(), data);
            return *this;
        }
    
        BlobBufBuilder &add(std::string_view name, std::string_view data) {
            blobmsg_add_string(&bb, name.data(), data.data());
            return *this;
        }
    
        const blob_buf &get() const {
            return bb;
        }
    };

#endif