
/**
 * @file joypad_accessory.c
 * @author Christopher Bonhage (me@christopherbonhage.com)
<<<<<<< HEAD
 * @brief Joypad accessory helpers
=======
 * @brief Joypad Accessory helpers
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
 * @ingroup joypad
 */

#include <string.h>
<<<<<<< HEAD

#include "debug.h"
#include "interrupt.h"
#include "kernel/kernel_internal.h"
#include "kirq.h"
#include "joypad_internal.h"

static void joypad_accessory_detect_read_callback(uint64_t *out_dwords, void *ctx);
static void joypad_accessory_detect_write_callback(uint64_t *out_dwords, void *ctx);
static void joypad_transfer_pak_enable_read_callback(uint64_t *out_dwords, void *ctx);
static void joypad_transfer_pak_enable_write_callback(uint64_t *out_dwords, void *ctx);
static void joypad_transfer_pak_load_read_callback(uint64_t *out_dwords, void *ctx);
static void joypad_transfer_pak_load_write_callback(uint64_t *out_dwords, void *ctx);
static void joypad_transfer_pak_store_read_callback(uint64_t *out_dwords, void *ctx);
static void joypad_transfer_pak_store_write_callback(uint64_t *out_dwords, void *ctx);
static void joypad_transfer_pak_wait_timer_callback(int ovfl, void *ctx);

/**
 * @addtogroup joypad
 * @{
 */

/**
 * @brief Determine whether the accessory read command was successful. Retry if necessary.
 * 
 * @param port Joypad port number (#joypad_port_t)
 * @param[in] cmd Joybus "N64 Accessory Read" command output
 * @param[in] retry_callback Callback to use if the command needs to be retried.
 * @param[in,out] retry_ctx Context to pass to the retry callback.
 * 
 * @retval true The accessory read command failed
 * @retval false The accessory read command succeeded
 */
static bool joypad_accessory_check_read_crc_error(
    joypad_port_t port,
    const joybus_cmd_n64_accessory_read_port_t *cmd,
=======
#include <libdragon.h>

#include "joybus_n64_accessory.h"
#include "joypad_accessory.h"
#include "joypad_internal.h"

static void joypad_n64_transfer_pak_wait_timer_callback(int ovfl, void *ctx);
static void joypad_accessory_detect_read_callback(uint64_t *out_dwords, void *ctx);
static void joypad_accessory_detect_write_callback(uint64_t *out_dwords, void *ctx);
static void joypad_n64_transfer_pak_enable_read_callback(uint64_t *out_dwords, void *ctx);
static void joypad_n64_transfer_pak_enable_write_callback(uint64_t *out_dwords, void *ctx);
static void joypad_n64_transfer_pak_load_read_callback(uint64_t *out_dwords, void *ctx);
static void joypad_n64_transfer_pak_load_write_callback(uint64_t *out_dwords, void *ctx);
static void joypad_n64_transfer_pak_store_read_callback(uint64_t *out_dwords, void *ctx);
static void joypad_n64_transfer_pak_store_write_callback(uint64_t *out_dwords, void *ctx);

static bool joypad_accessory_read_crc_error_check(
    joypad_port_t port,
    const joybus_cmd_n64_accessory_read_port_t *recv_cmd,
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
    joybus_callback_t retry_callback,
    void *retry_ctx
)
{
    volatile joypad_device_hot_t *device = &joypad_devices_hot[port];
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
<<<<<<< HEAD
    int crc_status = joybus_accessory_compare_data_crc(cmd->recv.data, cmd->recv.data_crc);
    switch (crc_status)
    {
        case JOYBUS_ACCESSORY_IO_STATUS_OK:
=======
    int crc_status = joybus_n64_accessory_data_crc_compare(recv_cmd->data, recv_cmd->data_crc);
    switch (crc_status)
    {
        case JOYBUS_N64_ACCESSORY_DATA_CRC_STATUS_OK:
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        {
            // Read operation was successful!
            accessory->error = JOYPAD_ACCESSORY_ERROR_NONE;
            return false;
        }
<<<<<<< HEAD
        case JOYBUS_ACCESSORY_IO_STATUS_NO_PAK:
=======
        case JOYBUS_N64_ACCESSORY_DATA_CRC_STATUS_NO_PAK:
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        {
            // Accessory is no longer connected!
            device->rumble_method = JOYPAD_RUMBLE_METHOD_NONE;
            device->rumble_active = false;
            accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
            accessory->type = JOYPAD_ACCESSORY_TYPE_NONE;
<<<<<<< HEAD
            accessory->status = JOYBUS_IDENTIFY_STATUS_ACCESSORY_ABSENT;
            accessory->error = JOYPAD_ACCESSORY_ERROR_ABSENT;
            return true;
        }
        case JOYBUS_ACCESSORY_IO_STATUS_BAD_CRC:
=======
            accessory->status = JOYBUS_IDENTIFY_STATUS_N64_ACCESSORY_ABSENT;
            accessory->error = JOYPAD_ACCESSORY_ERROR_ABSENT;
            return true;
        }
        case JOYBUS_N64_ACCESSORY_DATA_CRC_STATUS_MISMATCH:
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        {
            size_t retries = accessory->retries;
            if (retries < JOYPAD_ACCESSORY_RETRY_LIMIT)
            {
                // Retry: Bad communication with the accessory
                accessory->retries = retries + 1;
                accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
<<<<<<< HEAD
                uint16_t retry_addr = cmd->send.addr_checksum;
                retry_addr &= JOYBUS_ACCESSORY_ADDR_MASK_OFFSET;
                joybus_accessory_read_async(
=======
                uint16_t retry_addr = recv_cmd->addr_checksum;
                retry_addr &= JOYBUS_N64_ACCESSORY_ADDR_MASK_OFFSET;
                joybus_n64_accessory_read_async(
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
                    port, retry_addr,
                    retry_callback, retry_ctx
                );
                return true;
            }
            else
            {
                // Retry limit exceeded; read failed
                accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
                accessory->error = JOYPAD_ACCESSORY_ERROR_CHECKSUM;
                return true;
            }
        }
        default:
        {
            // This should never happen!
<<<<<<< HEAD
            assertf(false, "Unknown joybus_accessory_io_status_t value: %d", crc_status);
=======
            assertf(false, "Unknown N64 Accessory CRC comparison result: %d", crc_status);
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
            accessory->error = JOYPAD_ACCESSORY_ERROR_UNKNOWN;
            return true;
        }
    }
}

<<<<<<< HEAD
/**
 * @brief Determine whether the accessory write command was successful. Retry if necessary.
 * 
 * @param port Joypad port number (#joypad_port_t)
 * @param[in] cmd Joybus "N64 Accessory Write" command output
 * @param[in] retry_callback Callback to use if the command needs to be retried.
 * @param[in,out] retry_ctx Context to pass to the retry callback.
 * 
 * @retval true The accessory write command failed
 * @retval false The accessory write command succeeded
 */
static bool joypad_accessory_check_write_crc_error(
    joypad_port_t port,
    const joybus_cmd_n64_accessory_write_port_t *cmd,
=======
static bool joypad_accessory_write_crc_error_check(
    joypad_port_t port,
    const joybus_cmd_n64_accessory_write_port_t *recv_cmd,
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
    joybus_callback_t retry_callback,
    void *retry_ctx
)
{
    volatile joypad_device_hot_t *device = &joypad_devices_hot[port];
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
<<<<<<< HEAD
    int crc_status = joybus_accessory_compare_data_crc(cmd->send.data, cmd->recv.data_crc);
    switch (crc_status)
    {
        case JOYBUS_ACCESSORY_IO_STATUS_OK:
=======
    int crc_status = joybus_n64_accessory_data_crc_compare(recv_cmd->data, recv_cmd->data_crc);
    switch (crc_status)
    {
        case JOYBUS_N64_ACCESSORY_DATA_CRC_STATUS_OK:
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        {
            // Write operation was successful!
            // Intentionally preserve accessory status in this case
            accessory->error = JOYPAD_ACCESSORY_ERROR_NONE;
            return false;
        }
<<<<<<< HEAD
        case JOYBUS_ACCESSORY_IO_STATUS_NO_PAK:
=======
        case JOYBUS_N64_ACCESSORY_DATA_CRC_STATUS_NO_PAK:
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        {
            // Accessory is no longer connected!
            device->rumble_method = JOYPAD_RUMBLE_METHOD_NONE;
            device->rumble_active = false;
            accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
            accessory->type = JOYPAD_ACCESSORY_TYPE_NONE;
<<<<<<< HEAD
            accessory->status = JOYBUS_IDENTIFY_STATUS_ACCESSORY_ABSENT;
            accessory->error = JOYPAD_ACCESSORY_ERROR_ABSENT;
            return true;
        }
        case JOYBUS_ACCESSORY_IO_STATUS_BAD_CRC:
=======
            accessory->status = JOYBUS_IDENTIFY_STATUS_N64_ACCESSORY_ABSENT;
            accessory->error = JOYPAD_ACCESSORY_ERROR_ABSENT;
            return true;
        }
        case JOYBUS_N64_ACCESSORY_DATA_CRC_STATUS_MISMATCH:
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        {
            size_t retries = accessory->retries;
            if (retries < JOYPAD_ACCESSORY_RETRY_LIMIT)
            {
                // Retry: Bad communication with the accessory
                // Intentionally preserve accessory status in this case
                accessory->retries = retries + 1;
                accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
<<<<<<< HEAD
                uint16_t retry_addr = cmd->send.addr_checksum;
                retry_addr &= JOYBUS_ACCESSORY_ADDR_MASK_OFFSET;
                joybus_accessory_write_async(
                    port, retry_addr, cmd->send.data,
=======
                uint16_t retry_addr = recv_cmd->addr_checksum;
                retry_addr &= JOYBUS_N64_ACCESSORY_ADDR_MASK_OFFSET;
                joybus_n64_accessory_write_async(
                    port, retry_addr, recv_cmd->data,
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
                    retry_callback, retry_ctx
                );
                return true;
            }
            else
            {
                // Retry limit exceeded; write failed
                accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
                accessory->error = JOYPAD_ACCESSORY_ERROR_CHECKSUM;
                return true;
            }
        }
        default:
        {
            // This should never happen!
<<<<<<< HEAD
            assertf(false, "Unknown joybus_accessory_io_status_t value: %d", crc_status);
=======
            assertf(false, "Unknown N64 Accessory CRC comparison result: %d", crc_status);
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
            accessory->error = JOYPAD_ACCESSORY_ERROR_UNKNOWN;
            return true;
        }
    }
}

<<<<<<< HEAD
void joypad_accessory_reset(joypad_port_t port)
{
    ASSERT_JOYPAD_PORT_VALID(port);
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
    joypad_accessory_state_t state = accessory->state;
    timer_link_t *tpak_wait_timer = accessory->transfer_pak_wait_timer;
    joypad_accessory_io_callback_t io_callback = accessory->io.callback;
    void *io_callback_ctx = accessory->io.ctx;

    // Stop the Transfer Pak wait timer if it is running
    if (tpak_wait_timer) { stop_timer(tpak_wait_timer); }

    // Clear the accessory state
    memset((void *)accessory, 0, sizeof(*accessory));

    // Restore Transfer Pak wait timer pointer so that it can be re-used
    accessory->transfer_pak_wait_timer = tpak_wait_timer;

    // Resolve async accessory read/write callback
    if( (
        state == JOYPAD_ACCESSORY_STATE_READ ||
        state == JOYPAD_ACCESSORY_STATE_WRITE
        ) && io_callback != NULL
    )
    {
        io_callback( JOYPAD_ACCESSORY_ERROR_ABSENT, io_callback_ctx );
    }
}

/**
 * @brief Initialize the Transfer Pak wait timer if necessary.
 * 
 * @param port Joypad port number (#joypad_port_t)
 */
void joypad_transfer_pak_wait_timer_init(joypad_port_t port)
{
    ASSERT_JOYPAD_PORT_VALID(port);
=======

void joypad_n64_transfer_pak_wait_timer_init(joypad_port_t port)
{
    ASSERT_JOYBUS_CONTROLLER_PORT_VALID(port);
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
    // Ensure there is a disabled timer ready to restart:
    if (!accessory->transfer_pak_wait_timer)
    {
        // The Transfer Pak takes about 200 milliseconds to fully power-on
        // after being probed; sadly, we must use a hard-coded delay
        accessory->transfer_pak_wait_timer = new_timer_context(
            TIMER_TICKS(200 * 1000), TF_ONE_SHOT | TF_DISABLED,
<<<<<<< HEAD
            joypad_transfer_pak_wait_timer_callback, (void *)port
=======
            joypad_n64_transfer_pak_wait_timer_callback, (void *)port
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        );
    }
}

<<<<<<< HEAD
/**
 * @brief Callback for the Transfer Pak wait timer.
 * 
 * @param ovfl Timer overflow ticks
 * @param[in,out] ctx Opaque pointer to the Transfer Pak wait timer context
 */
static void joypad_transfer_pak_wait_timer_callback(int ovfl, void *ctx)
=======
static void joypad_n64_transfer_pak_wait_timer_callback(int ovfl, void *ctx)
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
{
    joypad_port_t port = (joypad_port_t)ctx;
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
    joypad_accessory_state_t state = accessory->state;
<<<<<<< HEAD

    // Cancel accessory detection during reset
    if( exception_reset_time() > 0 )
    {
        accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
        accessory->type = JOYPAD_ACCESSORY_TYPE_UNKNOWN;
        accessory->error = JOYPAD_ACCESSORY_ERROR_UNKNOWN;
    }
    else if (state == JOYPAD_ACCESSORY_STATE_TRANSFER_ENABLE_PROBE_WAIT)
    {
        uint8_t write_data[JOYBUS_ACCESSORY_DATA_SIZE];
        memset(write_data, JOYBUS_TRANSFER_PAK_STATUS_ACCESS, sizeof(write_data));
        accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_ENABLE_STATUS_WRITE;
        accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
        accessory->retries = 0;
        joybus_accessory_write_async(
            port, JOYBUS_ACCESSORY_ADDR_TRANSFER_STATUS, write_data,
            joypad_transfer_pak_enable_write_callback, ctx
=======
    if (state == JOYPAD_ACCESSORY_STATE_TRANSFER_ENABLE_PROBE_WAIT)
    {
        uint8_t write_data[JOYBUS_N64_ACCESSORY_DATA_SIZE];
        memset(write_data, JOYBUS_N64_TRANSFER_PAK_STATUS_ACCESS, sizeof(write_data));
        accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_ENABLE_STATUS_WRITE;
        accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
        accessory->retries = 0;
        joybus_n64_accessory_write_async(
            port, JOYBUS_N64_ACCESSORY_ADDR_TRANSFER_STATUS, write_data,
            joypad_n64_transfer_pak_enable_write_callback, ctx
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        );
    }
    else if (state == JOYPAD_ACCESSORY_STATE_TRANSFER_ENABLE_STATUS_WAIT)
    {
        accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_ENABLE_STATUS_READ;
        accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
        accessory->retries = 0;
<<<<<<< HEAD
        joybus_accessory_read_async(
            port, JOYBUS_ACCESSORY_ADDR_TRANSFER_STATUS,
            joypad_transfer_pak_enable_read_callback, ctx
=======
        joybus_n64_accessory_read_async(
            port, JOYBUS_N64_ACCESSORY_ADDR_TRANSFER_STATUS,
            joypad_n64_transfer_pak_enable_read_callback, ctx
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        );
    }
}

<<<<<<< HEAD
/**
 * @brief Callback for the accessory read commands used by #joypad_accessory_detect_async.
 * 
 * @param out_dwords Joybus output block
 * @param ctx Opaque pointer used to pass the Joypad port number
 */
=======
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
static void joypad_accessory_detect_read_callback(uint64_t *out_dwords, void *ctx)
{
    const uint8_t *out_bytes = (void *)out_dwords;
    joypad_port_t port = (joypad_port_t)ctx;
    volatile joypad_device_hot_t *device = &joypad_devices_hot[port];
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
    joypad_accessory_state_t state = accessory->state;
<<<<<<< HEAD
    if (!joypad_accessory_state_is_detecting(state))
    {
        return; // Unexpected accessory state!
    }

    // Cancel accessory detection during reset
    if( exception_reset_time() > 0 )
    {
        accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
        accessory->type = JOYPAD_ACCESSORY_TYPE_UNKNOWN;
        accessory->error = JOYPAD_ACCESSORY_ERROR_UNKNOWN;
        return;
    }

    uint8_t write_data[JOYBUS_ACCESSORY_DATA_SIZE];
    const joybus_cmd_n64_accessory_read_port_t *cmd =
        (void *)&out_bytes[port + JOYBUS_COMMAND_METADATA_SIZE];
    joybus_callback_t retry_callback = joypad_accessory_detect_read_callback;
    if (joypad_accessory_check_read_crc_error(port, cmd, retry_callback, ctx))
    {
        return; // Accessory communication error!
    }
    else if (state == JOYPAD_ACCESSORY_STATE_DETECT_CPAK_LABEL_BACKUP)
    {
        memcpy((void *)accessory->cpak_label_backup, cmd->recv.data, sizeof(cmd->recv.data));
        // Step 2C: Overwrite the Controller Pak "label" area
        for (size_t i = 0; i < sizeof(write_data); ++i) write_data[i] = i;
        accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_CPAK_LABEL_WRITE;
        accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
        accessory->retries = 0;
        joybus_accessory_write_async(
            port, JOYBUS_ACCESSORY_ADDR_LABEL, write_data,
            joypad_accessory_detect_write_callback, ctx
        );
    }
    else if (state == JOYPAD_ACCESSORY_STATE_DETECT_CPAK_LABEL_READ)
    {
        // Compare the expected label with what was actually read back
        for (size_t i = 0; i < sizeof(write_data); ++i) write_data[i] = i;
        if (memcmp(cmd->recv.data, write_data, sizeof(write_data)) == 0)
        {
            // Step 2E: Restore the Controller Pak "label" area
            memcpy(write_data, (void *)accessory->cpak_label_backup, sizeof(write_data));
            accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_CPAK_LABEL_RESTORE;
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
            joybus_accessory_write_async(
                port, JOYBUS_ACCESSORY_ADDR_LABEL, write_data,
                joypad_accessory_detect_write_callback, ctx
            );
=======
    if (!joypad_accessory_state_is_detecting(state)) return;

    uint8_t write_data[JOYBUS_N64_ACCESSORY_DATA_SIZE];
    const joybus_cmd_n64_accessory_read_port_t *recv_cmd = (void *)&out_bytes[port];
    joybus_callback_t retry_callback = joypad_accessory_detect_read_callback;
    if (joypad_accessory_read_crc_error_check(port, recv_cmd, retry_callback, ctx))
    {
        return; // Accessory communication error!
    }
    else if (state == JOYPAD_ACCESSORY_STATE_DETECT_LABEL_READ)
    {
        // Compare the expected label with what was actually read back
        for (size_t i = 0; i < sizeof(write_data); ++i) write_data[i] = i;
        if (memcmp(recv_cmd->data, write_data, sizeof(write_data)) == 0)
        {
            // Success: Label write persisted; this appears to be a Controller Pak
            accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
            accessory->type = JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK;
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        }
        else
        {
            // Step 3A: Write probe value to detect Rumble Pak
<<<<<<< HEAD
            memset(write_data, JOYBUS_ACCESSORY_PROBE_RUMBLE_PAK, sizeof(write_data));
            accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_RUMBLE_PROBE_WRITE;
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
            joybus_accessory_write_async(
                port, JOYBUS_ACCESSORY_ADDR_PROBE, write_data,
=======
            memset(write_data, JOYBUS_N64_ACCESSORY_PROBE_RUMBLE_PAK, sizeof(write_data));
            accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_RUMBLE_PROBE_WRITE;
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
            joybus_n64_accessory_write_async(
                port, JOYBUS_N64_ACCESSORY_ADDR_PROBE, write_data,
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
                joypad_accessory_detect_write_callback, ctx
            );
        }
    }
    else if (state == JOYPAD_ACCESSORY_STATE_DETECT_RUMBLE_PROBE_READ)
    {
<<<<<<< HEAD
        uint8_t probe_value = cmd->recv.data[0];
        if (probe_value == JOYBUS_ACCESSORY_PROBE_RUMBLE_PAK)
=======
        uint8_t probe_value = recv_cmd->data[0];
        if (probe_value == JOYBUS_N64_ACCESSORY_PROBE_RUMBLE_PAK)
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        {
            // Success: Probe reports that this is a Rumble Pak
            accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
            accessory->type = JOYPAD_ACCESSORY_TYPE_RUMBLE_PAK;
            device->rumble_method = JOYPAD_RUMBLE_METHOD_N64_RUMBLE_PAK;
        }
<<<<<<< HEAD
        else if (probe_value == JOYBUS_ACCESSORY_PROBE_BIO_SENSOR)
=======
        else if (probe_value == JOYBUS_N64_ACCESSORY_PROBE_BIO_SENSOR)
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        {
            // Success: Bio Sensor responds to all reads with probe value
            accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
            accessory->type = JOYPAD_ACCESSORY_TYPE_BIO_SENSOR;
        }
        else
        {
            // Step 4A: Write probe value to detect Transfer Pak
<<<<<<< HEAD
            memset(write_data, JOYBUS_ACCESSORY_PROBE_TRANSFER_PAK_ON, sizeof(write_data));
            accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_TRANSFER_PROBE_ON;
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
            joybus_accessory_write_async(
                port, JOYBUS_ACCESSORY_ADDR_PROBE, write_data,
=======
            memset(write_data, JOYBUS_N64_ACCESSORY_PROBE_TRANSFER_PAK_ON, sizeof(write_data));
            accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_TRANSFER_PROBE_ON;
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
            joybus_n64_accessory_write_async(
                port, JOYBUS_N64_ACCESSORY_ADDR_PROBE, write_data,
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
                joypad_accessory_detect_write_callback, ctx
            );
        }
    }
    else if (state == JOYPAD_ACCESSORY_STATE_DETECT_TRANSFER_PROBE_READ)
    {
<<<<<<< HEAD
        uint8_t probe_value = cmd->recv.data[0];
        if (probe_value == JOYBUS_ACCESSORY_PROBE_TRANSFER_PAK_ON)
        {
            // Step 4C: Write probe value to turn off Transfer Pak
            memset(write_data, JOYBUS_ACCESSORY_PROBE_TRANSFER_PAK_OFF, sizeof(write_data));
            accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_TRANSFER_PROBE_OFF;
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
            joybus_accessory_write_async(
                port, JOYBUS_ACCESSORY_ADDR_PROBE, write_data,
=======
        uint8_t probe_value = recv_cmd->data[0];
        if (probe_value == JOYBUS_N64_ACCESSORY_PROBE_TRANSFER_PAK_ON)
        {
            // Step 4C: Write probe value to turn off Transfer Pak
            memset(write_data, JOYBUS_N64_ACCESSORY_PROBE_TRANSFER_PAK_OFF, sizeof(write_data));
            accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_TRANSFER_PROBE_OFF;
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
            joybus_n64_accessory_write_async(
                port, JOYBUS_N64_ACCESSORY_ADDR_PROBE, write_data,
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
                joypad_accessory_detect_write_callback, (void *)port
            );
        }
        else
        {
            // Step 5A: Write probe value to detect Snap Station
<<<<<<< HEAD
            memset(write_data, JOYBUS_ACCESSORY_PROBE_SNAP_STATION, sizeof(write_data));
            accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_SNAP_PROBE_WRITE;
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
            joybus_accessory_write_async(
                port, JOYBUS_ACCESSORY_ADDR_PROBE, write_data,
=======
            memset(write_data, JOYBUS_N64_ACCESSORY_PROBE_SNAP_STATION, sizeof(write_data));
            accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_SNAP_PROBE_WRITE;
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
            joybus_n64_accessory_write_async(
                port, JOYBUS_N64_ACCESSORY_ADDR_PROBE, write_data,
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
                joypad_accessory_detect_write_callback, ctx
            );
        }
    }
    else if (state == JOYPAD_ACCESSORY_STATE_DETECT_SNAP_PROBE_READ)
    {
<<<<<<< HEAD
        uint8_t probe_value = cmd->recv.data[0];
        if (probe_value == JOYBUS_ACCESSORY_PROBE_SNAP_STATION)
=======
        uint8_t probe_value = recv_cmd->data[0];
        if (probe_value == JOYBUS_N64_ACCESSORY_PROBE_SNAP_STATION)
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        {
            // Success: Probe reports that this is a Snap Station
            accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
            accessory->type = JOYPAD_ACCESSORY_TYPE_SNAP_STATION;
        }
        else
        {
            // Failure: Unable to determine which accessory is connected
            accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
            accessory->type = JOYPAD_ACCESSORY_TYPE_UNKNOWN;
            accessory->transfer_pak_status.raw = 0x00;
        }
    }
}

<<<<<<< HEAD
/**
 * @brief Callback for the accessory write commands used by #joypad_accessory_detect_async.
 * 
 * @param out_dwords Joybus output block
 * @param ctx Opaque pointer used to pass the Joypad port number
 */
=======
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
static void joypad_accessory_detect_write_callback(uint64_t *out_dwords, void *ctx)
{
    const uint8_t *out_bytes = (void *)out_dwords;
    joypad_port_t port = (joypad_port_t)ctx;
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
    joypad_accessory_state_t state = accessory->state;
<<<<<<< HEAD
    if (!joypad_accessory_state_is_detecting(state))
    {
        return; // Unexpected accessory state!
    }

    // Cancel accessory detection during reset
    if( exception_reset_time() > 0 )
    {
        accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
        accessory->type = JOYPAD_ACCESSORY_TYPE_UNKNOWN;
        accessory->error = JOYPAD_ACCESSORY_ERROR_UNKNOWN;
        return;
    }

    const joybus_cmd_n64_accessory_write_port_t *cmd =
        (void *)&out_bytes[port + JOYBUS_COMMAND_METADATA_SIZE];
    joybus_callback_t retry_callback = joypad_accessory_detect_write_callback;
    if (joypad_accessory_check_write_crc_error(port, cmd, retry_callback, ctx))
=======
    if (!joypad_accessory_state_is_detecting(state)) return;

    const joybus_cmd_n64_accessory_write_port_t *recv_cmd = (void *)&out_bytes[port];
    joybus_callback_t retry_callback = joypad_accessory_detect_write_callback;
    if (joypad_accessory_write_crc_error_check(port, recv_cmd, retry_callback, ctx))
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
    {
        return; // Accessory communication error!
    }
    else if (state == JOYPAD_ACCESSORY_STATE_DETECT_INIT)
    {
        // Transfer Pak has been turned off; reset Transfer Pak status
        accessory->transfer_pak_status.raw = 0x00;
<<<<<<< HEAD
        // Step 2A: Set Controller Pak "linear paging bank" to 0
        uint8_t data[JOYBUS_ACCESSORY_DATA_SIZE] = {0};
        accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_CPAK_BANK_WRITE;
        accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
        accessory->retries = 0;
        joybus_accessory_write_async(
            port, JOYPAD_CONTROLLER_PAK_BANK_SWITCH_ADDRESS, data,
            joypad_accessory_detect_write_callback, ctx
        );
    }
    else if (state == JOYPAD_ACCESSORY_STATE_DETECT_CPAK_BANK_WRITE)
    {
        // Step 2B: Backup the Controller Pak "label" area
        accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_CPAK_LABEL_BACKUP;
        accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
        accessory->retries = 0;
        joybus_accessory_read_async(
            port, JOYBUS_ACCESSORY_ADDR_LABEL,
            joypad_accessory_detect_read_callback, ctx
        );
    }
    else if (state == JOYPAD_ACCESSORY_STATE_DETECT_CPAK_LABEL_WRITE)
    {
        // Step 2D: Read back the "label" area to detect Controller Pak
        accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_CPAK_LABEL_READ;
        accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
        accessory->retries = 0;
        joybus_accessory_read_async(
            port, JOYBUS_ACCESSORY_ADDR_LABEL,
            joypad_accessory_detect_read_callback, ctx
        );
    }
    else if (state == JOYPAD_ACCESSORY_STATE_DETECT_CPAK_LABEL_RESTORE)
    {
        // Success: Controller Pak detected
        accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
        accessory->type = JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK;
    }
=======
        // Step 2A: Overwrite "label" area to detect Controller Pak
        uint8_t data[JOYBUS_N64_ACCESSORY_DATA_SIZE];
        for (size_t i = 0; i < sizeof(data); ++i) data[i] = i;
        accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_LABEL_WRITE;
        accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
        accessory->retries = 0;
        joybus_n64_accessory_write_async(
            port, JOYBUS_N64_ACCESSORY_ADDR_LABEL, data,
            joypad_accessory_detect_write_callback, ctx
        );
    }
    else if (state == JOYPAD_ACCESSORY_STATE_DETECT_LABEL_WRITE)
    {
        // Step 2B: Read back the "label" area to detect Controller Pak
        accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_LABEL_READ;
        accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
        accessory->retries = 0;
        joybus_n64_accessory_read_async(
            port, JOYBUS_N64_ACCESSORY_ADDR_LABEL,
            joypad_accessory_detect_read_callback, ctx
        );
    }
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
    else if (state == JOYPAD_ACCESSORY_STATE_DETECT_RUMBLE_PROBE_WRITE)
    {
        // Step 3B: Read probe value to detect Rumble Pak
        accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_RUMBLE_PROBE_READ;
        accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
        accessory->retries = 0;
<<<<<<< HEAD
        joybus_accessory_read_async(
            port, JOYBUS_ACCESSORY_ADDR_PROBE,
=======
        joybus_n64_accessory_read_async(
            port, JOYBUS_N64_ACCESSORY_ADDR_PROBE,
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            joypad_accessory_detect_read_callback, ctx
        );
    }
    else if (state == JOYPAD_ACCESSORY_STATE_DETECT_TRANSFER_PROBE_ON)
    {
        // Step 4B: Read probe value to detect Transfer Pak
        accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_TRANSFER_PROBE_READ;
        accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
        accessory->retries = 0;
<<<<<<< HEAD
        joybus_accessory_read_async(
            port, JOYBUS_ACCESSORY_ADDR_PROBE,
=======
        joybus_n64_accessory_read_async(
            port, JOYBUS_N64_ACCESSORY_ADDR_PROBE,
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            joypad_accessory_detect_read_callback, ctx
        );
    }
    else if (state == JOYPAD_ACCESSORY_STATE_DETECT_TRANSFER_PROBE_OFF)
    {
        // Success: Transfer Pak has been probed and powered off
        accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
        accessory->type = JOYPAD_ACCESSORY_TYPE_TRANSFER_PAK;
        accessory->transfer_pak_status.power = 0;
    }
    else if (state == JOYPAD_ACCESSORY_STATE_DETECT_SNAP_PROBE_WRITE)
    {
        // Step 5B: Read probe value to detect Snap Station
        accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_SNAP_PROBE_READ;
        accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
        accessory->retries = 0;
<<<<<<< HEAD
        joybus_accessory_read_async(
            port, JOYBUS_ACCESSORY_ADDR_PROBE,
=======
        joybus_n64_accessory_read_async(
            port, JOYBUS_N64_ACCESSORY_ADDR_PROBE,
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            joypad_accessory_detect_read_callback, ctx
        );
    }
}

<<<<<<< HEAD
void joypad_accessory_detect_async(joypad_port_t port)
{
    // Disable accessory detection during reset
    if( exception_reset_time() > 0 ) return;

    ASSERT_JOYPAD_PORT_VALID(port);
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
    // Ensure Transfer Pak wait timer has been initialized
    if (!accessory->transfer_pak_wait_timer)
    {
        joypad_transfer_pak_wait_timer_init(port);
    }
=======
/**
 * @brief Detect which accessory is inserted in an N64 controller.
 * 
 * Step 1: Ensure Transfer Pak is turned off
 * Step 2A: Overwrite "label" area to detect Controller Pak
 * Step 2B: Read back the "label" area to detect Controller Pak
 * Step 3A: Write probe value to detect Rumble Pak
 * Step 3B: Read probe value to detect Rumble Pak
 * Step 4A: Write probe value to detect Transfer Pak
 * Step 4B: Read probe value to detect Transfer Pak
 * Step 4C: Write probe value to turn off Transfer Pak
 * Step 5A: Write probe value to detect Snap Station
 * Step 5B: Read probe value to detect Snap Station
 * 
 * @param[in] port Which controller port to detect the accessory on
 */
void joypad_accessory_detect_async(joypad_port_t port)
{
    ASSERT_JOYBUS_CONTROLLER_PORT_VALID(port);
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
    // Ensure Transfer Pak wait timer has been initialized
    if (!accessory->transfer_pak_wait_timer)
        joypad_n64_transfer_pak_wait_timer_init(port);
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
    // Don't interrupt other accessory operations if they are still running
    if (accessory->state == JOYPAD_ACCESSORY_STATE_IDLE)
    {
        // Step 1: Ensure Transfer Pak is turned off
<<<<<<< HEAD
        uint8_t data[JOYBUS_ACCESSORY_DATA_SIZE];
        memset(data, JOYBUS_ACCESSORY_PROBE_TRANSFER_PAK_OFF, sizeof(data));
        accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_INIT;
        accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
        accessory->retries = 0;
        joybus_accessory_write_async(
            port, JOYBUS_ACCESSORY_ADDR_PROBE, data,
=======
        uint8_t data[JOYBUS_N64_ACCESSORY_DATA_SIZE];
        memset(data, JOYBUS_N64_ACCESSORY_PROBE_TRANSFER_PAK_OFF, sizeof(data));
        accessory->state = JOYPAD_ACCESSORY_STATE_DETECT_INIT;
        accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
        accessory->retries = 0;
        joybus_n64_accessory_write_async(
            port, JOYBUS_N64_ACCESSORY_ADDR_PROBE, data,
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            joypad_accessory_detect_write_callback, (void *)port
        );
    }
}

<<<<<<< HEAD
/**
 * @brief Callback for the accessory write commands used by #joypad_rumble_pak_toggle_async.
 * 
 * @param out_dwords Joybus output block
 * @param ctx Opaque pointer used to pass the Joypad port number
 */
static void joypad_rumble_pak_motor_write_callback(uint64_t *out_dwords, void *ctx)
=======
static void joypad_n64_rumble_pak_motor_write_callback(uint64_t *out_dwords, void *ctx)
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
{
    const uint8_t *out_bytes = (void *)out_dwords;
    joypad_port_t port = (joypad_port_t)ctx;
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
    joypad_accessory_state_t state = accessory->state;
<<<<<<< HEAD
    if (state != JOYPAD_ACCESSORY_STATE_RUMBLE_WRITE)
    {
        return; // Unexpected accessory state!
    }

    // Do not attempt to retry rumble motor control commands during reset
    if( exception_reset_time() > 0 )
    {
        accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
        return;
    }

    const joybus_cmd_n64_accessory_write_port_t *cmd =
        (void *)&out_bytes[port + JOYBUS_COMMAND_METADATA_SIZE];
    joybus_callback_t retry_callback = joypad_rumble_pak_motor_write_callback;
    if (!joypad_accessory_check_write_crc_error(port, cmd, retry_callback, ctx))
=======
    if (state != JOYPAD_ACCESSORY_STATE_RUMBLE_WRITE) return;

    const joybus_cmd_n64_accessory_write_port_t *recv_cmd = (void *)&out_bytes[port];
    joybus_callback_t retry_callback = joypad_n64_rumble_pak_motor_write_callback;
    if (!joypad_accessory_write_crc_error_check(port, recv_cmd, retry_callback, ctx))
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
    {
        accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
    }
}

<<<<<<< HEAD
void joypad_rumble_pak_toggle_async(joypad_port_t port, bool active)
{
    // Disable rumble motor control during reset
    if( exception_reset_time() > 0 ) return;

=======
void joypad_n64_rumble_pak_motor_async(joypad_port_t port, bool active)
{
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
    volatile joypad_device_hot_t *device = &joypad_devices_hot[port];
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
    device->rumble_active = active;
    accessory->state = JOYPAD_ACCESSORY_STATE_RUMBLE_WRITE;
    accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
    accessory->retries = 0;
<<<<<<< HEAD
    uint8_t motor_data[JOYBUS_ACCESSORY_DATA_SIZE];
    memset(motor_data, active, sizeof(motor_data));
    joybus_accessory_write_async(
        port, JOYBUS_ACCESSORY_ADDR_RUMBLE_MOTOR, motor_data,
        joypad_rumble_pak_motor_write_callback, (void *)port
    );
}

static void joypad_accessory_read_callback(uint64_t *out_dwords, void *ctx)
{
    const uint8_t *out_bytes = (void *)out_dwords;
    joypad_port_t port = (joypad_port_t)ctx;
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
    volatile joypad_accessory_io_t *io = &accessory->io;
    joypad_accessory_state_t state = accessory->state;
    assert(state == JOYPAD_ACCESSORY_STATE_READ);

    const joybus_cmd_n64_accessory_read_port_t *cmd =
        (void *)&out_bytes[port + JOYBUS_COMMAND_METADATA_SIZE];
    if (!joypad_accessory_check_read_crc_error(port, cmd, joypad_accessory_read_callback, ctx))
    {
        int offset = io->cart_addr & 0x1F;
        int len = MIN(JOYBUS_ACCESSORY_DATA_SIZE - offset, io->end - io->cursor);
        memcpy(io->cursor, cmd->recv.data + offset, len);
        io->cursor += len;

        if (io->cursor < io->end)
        {
            // Read the next block of data
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
            uint16_t next_addr = cmd->send.addr_checksum & JOYBUS_ACCESSORY_ADDR_MASK_OFFSET;
            next_addr += JOYBUS_ACCESSORY_DATA_SIZE;
            io->cart_addr = next_addr;
            joybus_accessory_read_async(
                port, next_addr,
                joypad_accessory_read_callback, ctx
            );
            return;
        }

        // Read operation is complete
        accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
        accessory->error = JOYPAD_ACCESSORY_ERROR_NONE;
    }

    if (accessory->error != JOYPAD_ACCESSORY_ERROR_PENDING)
    {
        joypad_accessory_io_callback_t callback = io->callback;
        void *ctx = io->ctx;

        memset( (void *)io, 0, sizeof(*io) );
        if (callback != NULL)
            callback( accessory->error, ctx );
    }
}

static void joypad_accessory_write_callback(uint64_t *out_dwords, void *ctx)
{
    const uint8_t *out_bytes = (void *)out_dwords;
    joypad_port_t port = (joypad_port_t)ctx;
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
    volatile joypad_accessory_io_t *io = &accessory->io;
    joypad_accessory_state_t state = accessory->state;
    assert(state == JOYPAD_ACCESSORY_STATE_WRITE || state == JOYPAD_ACCESSORY_STATE_READ);
    int offset = io->cart_addr & 0x1F;
    int len = MIN(JOYBUS_ACCESSORY_DATA_SIZE - offset, io->end - io->cursor);

    if (state == JOYPAD_ACCESSORY_STATE_READ) {
        joybus_cmd_n64_accessory_read_port_t *cmd =
            (void *)&out_bytes[port + JOYBUS_COMMAND_METADATA_SIZE];
        if (!joypad_accessory_check_read_crc_error(port, cmd, joypad_accessory_write_callback, ctx))
        {
            // Merge the received data with the existing data
            memcpy(cmd->recv.data + offset, io->cursor, len);

            // Now issue a write command to the accessory to the same address
            // to store the data
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
            accessory->state = JOYPAD_ACCESSORY_STATE_WRITE;
            joybus_accessory_write_async(
                port, io->cart_addr, cmd->recv.data,
                joypad_accessory_write_callback, ctx
            );
            return;
        }
    } else {
        const joybus_cmd_n64_accessory_write_port_t *cmd =
            (void *)&out_bytes[port + JOYBUS_COMMAND_METADATA_SIZE];
        if (!joypad_accessory_check_write_crc_error(port, cmd, joypad_accessory_write_callback, ctx))
        {
            uint8_t *cursor = io->cursor += len;
            if (cursor < io->end)
            {
                // Read the next block of data
                accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
                accessory->retries = 0;
                uint16_t next_addr = cmd->send.addr_checksum & JOYBUS_ACCESSORY_ADDR_MASK_OFFSET;
                next_addr += JOYBUS_ACCESSORY_DATA_SIZE;
                io->cart_addr = next_addr;

                bool partial = (io->end - cursor) < JOYBUS_ACCESSORY_DATA_SIZE;
                if (partial) {
                    accessory->state = JOYPAD_ACCESSORY_STATE_READ;
                    joybus_accessory_read_async(port, next_addr, joypad_accessory_write_callback, ctx);
                } else {
                    joybus_accessory_write_async(port, next_addr, cursor, joypad_accessory_write_callback, ctx);
                }
                return;
            }

            // Read operation is complete
            accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
            accessory->error = JOYPAD_ACCESSORY_ERROR_NONE;
        }
    }

    if (accessory->error != JOYPAD_ACCESSORY_ERROR_PENDING)
    {
        joypad_accessory_io_callback_t callback = io->callback;
        void *ctx = io->ctx;

        memset( (void *)io, 0, sizeof(*io) );
        if (callback != NULL)
            callback( accessory->error, ctx );
    }
}

void joypad_accessory_xfer_async(
    joypad_port_t port,
    joypad_accessory_xfer_t xfer,
    uint16_t start_addr,
    void *dst,
    size_t len,
    joypad_accessory_io_callback_t callback,
    void *ctx
)
{
    // Disable accessory transfers during reset
    if( exception_reset_time() > 0 )
    {
        if (callback != NULL) callback( JOYPAD_ACCESSORY_ERROR_UNKNOWN, ctx );
        return;
    }

    ASSERT_JOYPAD_PORT_VALID(port);
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];

    // We can only handle one async transfer at a time. If the accessory is
    // busy, we will wait for it to become idle before starting the new transfer.
    kirq_wait_t w = kirq_begin_wait_si();
    while (accessory->state != JOYPAD_ACCESSORY_STATE_IDLE) {
        if (__kernel) kirq_wait(&w);
    }

    accessory->io = (joypad_accessory_io_t){
        .start = dst,
        .end = dst + len,
        .cursor = dst,
        .cart_addr = start_addr,
        .callback = callback,
        .ctx = ctx,
    };
    accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
    accessory->retries = 0;

    switch (xfer) {
    case JOYPAD_ACCESSORY_XFER_READ:
        accessory->state = JOYPAD_ACCESSORY_STATE_READ;
        joybus_accessory_read_async(
            port, start_addr & JOYBUS_ACCESSORY_ADDR_MASK_OFFSET,
            joypad_accessory_read_callback, (void *)port
        );
        break;
    case JOYPAD_ACCESSORY_XFER_WRITE:
        // If we're wriring
        if ((start_addr & 0x1F) == 0 && len >= JOYBUS_ACCESSORY_DATA_SIZE) {
            accessory->state = JOYPAD_ACCESSORY_STATE_WRITE;
            joybus_accessory_write_async(
                port, start_addr & JOYBUS_ACCESSORY_ADDR_MASK_OFFSET, dst,
                joypad_accessory_write_callback, (void *)port
            );
        } else {
            accessory->state = JOYPAD_ACCESSORY_STATE_READ;
            joybus_accessory_read_async(
                port, start_addr & JOYBUS_ACCESSORY_ADDR_MASK_OFFSET,
                joypad_accessory_write_callback, (void *)port
            );
        }
        break;
    }
}

joypad_accessory_error_t joypad_accessory_xfer(
    joypad_port_t port, 
    joypad_accessory_xfer_t xfer,
    uint16_t start_addr, 
    void *dst,
    size_t len)
{
    // Disable accessory transfers during reset
    if( exception_reset_time() > 0 ) return JOYPAD_ACCESSORY_ERROR_UNKNOWN;

    volatile bool done = false;
    volatile joypad_accessory_error_t error = JOYPAD_ACCESSORY_ERROR_NONE;

    void callback(joypad_accessory_error_t e, void *ctx)
    {
        error = e;
        done = true;
    }

    kirq_wait_t w = kirq_begin_wait_si();
    joypad_accessory_xfer_async(port, xfer, start_addr, dst, len, callback, NULL);

    while (!done) {
        if (__kernel) kirq_wait(&w);
    }

    return error;
}

joypad_accessory_error_t joypad_controller_pak_set_bank(joypad_port_t port, uint8_t bank)
{
    ASSERT_JOYPAD_PORT_VALID(port);
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];

    if (accessory->type != JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK)
    {
        return JOYPAD_ACCESSORY_ERROR_ABSENT; // This is not a Controller Pak!
    }
    
    uint8_t data[32];
    memset(data, bank, sizeof(data));
    return joypad_accessory_xfer(port, JOYPAD_ACCESSORY_XFER_WRITE,
        JOYPAD_CONTROLLER_PAK_BANK_SWITCH_ADDRESS, 
        data, sizeof(data));
}

/**
 * @brief Callback for the accessory read commands used by #joypad_transfer_pak_enable_async.
 * 
 * @param out_dwords Joybus output block
 * @param ctx Opaque pointer used to pass the Joypad port number
 */
static void joypad_transfer_pak_enable_read_callback(uint64_t *out_dwords, void *ctx)
=======
    uint8_t motor_data[JOYBUS_N64_ACCESSORY_DATA_SIZE];
    memset(motor_data, active, sizeof(motor_data));
    joybus_n64_accessory_write_async(
        port, JOYBUS_N64_ACCESSORY_ADDR_RUMBLE_MOTOR, motor_data,
        joypad_n64_rumble_pak_motor_write_callback, (void *)port
    );
}

static void joypad_n64_transfer_pak_enable_read_callback(uint64_t *out_dwords, void *ctx)
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
{
    const uint8_t *out_bytes = (void *)out_dwords;
    joypad_port_t port = (joypad_port_t)ctx;
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
    joypad_accessory_state_t state = accessory->state;
<<<<<<< HEAD
    if (!joypad_accessory_state_is_transfer_enabling(state))
    {
        return; // Unexpected accessory state!
    }

    const joybus_cmd_n64_accessory_read_port_t *cmd =
        (void *)&out_bytes[port + JOYBUS_COMMAND_METADATA_SIZE];
    joybus_callback_t retry_callback = joypad_transfer_pak_enable_read_callback;
    if (joypad_accessory_check_read_crc_error(port, cmd, retry_callback, ctx))
=======
    if (!joypad_accessory_state_is_transfer_enabling(state)) return;

    const joybus_cmd_n64_accessory_read_port_t *recv_cmd = (void *)&out_bytes[port];
    joybus_callback_t retry_callback = joypad_n64_transfer_pak_enable_read_callback;
    if (joypad_accessory_read_crc_error_check(port, recv_cmd, retry_callback, ctx))
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
    {
        return; // Accessory communication error!
    }
    else if (state == JOYPAD_ACCESSORY_STATE_TRANSFER_ENABLE_STATUS_READ)
    {
<<<<<<< HEAD
        accessory->transfer_pak_status.raw = cmd->recv.data[0];
=======
        accessory->transfer_pak_status.raw = recv_cmd->data[0];
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
    }
}

<<<<<<< HEAD
/**
 * @brief Callback for the accessory write commands used by #joypad_transfer_pak_enable_async.
 * 
 * @param out_dwords Joybus output block
 * @param ctx Opaque pointer used to pass the Joypad port number
 */
static void joypad_transfer_pak_enable_write_callback(uint64_t *out_dwords, void *ctx)
=======
static void joypad_n64_transfer_pak_enable_write_callback(uint64_t *out_dwords, void *ctx)
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
{
    const uint8_t *out_bytes = (void *)out_dwords;
    joypad_port_t port = (joypad_port_t)ctx;
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
    joypad_accessory_state_t state = accessory->state;
<<<<<<< HEAD
    if (!joypad_accessory_state_is_transfer_enabling(state))
    {
        return; // Unexpected accessory state!
    }

    const joybus_cmd_n64_accessory_write_port_t *cmd =
        (void *)&out_bytes[port + JOYBUS_COMMAND_METADATA_SIZE];
    joybus_callback_t retry_callback = joypad_transfer_pak_enable_write_callback;
    if (joypad_accessory_check_write_crc_error(port, cmd, retry_callback, ctx))
=======
    if (!joypad_accessory_state_is_transfer_enabling(state)) return;

    const joybus_cmd_n64_accessory_write_port_t *recv_cmd = (void *)&out_bytes[port];
    joybus_callback_t retry_callback = joypad_n64_transfer_pak_enable_write_callback;
    if (joypad_accessory_write_crc_error_check(port, recv_cmd, retry_callback, ctx))
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
    {
        return; // Accessory communication error!
    }
    else if (state == JOYPAD_ACCESSORY_STATE_TRANSFER_ENABLE_PROBE_WRITE)
    {
<<<<<<< HEAD
        if (cmd->send.data[0] == JOYBUS_ACCESSORY_PROBE_TRANSFER_PAK_ON)
=======
        if (recv_cmd->data[0] == JOYBUS_N64_ACCESSORY_PROBE_TRANSFER_PAK_ON)
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        {
            accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_ENABLE_PROBE_WAIT;
            timer_link_t *timer = accessory->transfer_pak_wait_timer;
            assertf(timer, "transfer_pak_wait_timer is NULL on port %d", port + 1);
            restart_timer(timer);
        }
        else
        {
            accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
            accessory->transfer_pak_status.raw = 0x00;
        }
    }
    else if (state == JOYPAD_ACCESSORY_STATE_TRANSFER_ENABLE_STATUS_WRITE)
    {
        accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_ENABLE_STATUS_WAIT;
        timer_link_t *timer = accessory->transfer_pak_wait_timer;
        assertf(timer, "transfer_pak_wait_timer is NULL on port %d", port + 1);
        restart_timer(timer);
    }
}

<<<<<<< HEAD
void joypad_transfer_pak_enable_async(joypad_port_t port, bool enabled)
{
    ASSERT_JOYPAD_PORT_VALID(port);
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];

    // Turn the Transfer Pak on or off with magic probe values
    uint8_t probe_value = enabled
        ? JOYBUS_ACCESSORY_PROBE_TRANSFER_PAK_ON
        : JOYBUS_ACCESSORY_PROBE_TRANSFER_PAK_OFF
        ;
    uint8_t write_data[JOYBUS_ACCESSORY_DATA_SIZE];
=======
void joypad_n64_transfer_pak_enable_async(joypad_port_t port, bool enabled)
{
    ASSERT_JOYBUS_CONTROLLER_PORT_VALID(port);
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
    
    // Turn the Transfer Pak on or off with magic probe values
    uint8_t probe_value = enabled
        ? JOYBUS_N64_ACCESSORY_PROBE_TRANSFER_PAK_ON
        : JOYBUS_N64_ACCESSORY_PROBE_TRANSFER_PAK_OFF
        ;
    uint8_t write_data[JOYBUS_N64_ACCESSORY_DATA_SIZE];
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
    memset(write_data, probe_value, sizeof(write_data));

    accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_ENABLE_PROBE_WRITE;
    accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
    accessory->retries = 0;
<<<<<<< HEAD
    joybus_accessory_write_async(
        port, JOYBUS_ACCESSORY_ADDR_PROBE, write_data,
        joypad_transfer_pak_enable_write_callback, (void *)port
    );
}

/**
 * @brief Callback for the accessory read commands used by #joypad_transfer_pak_load_async.
 * 
 * @param out_dwords Joybus output block
 * @param ctx Opaque pointer used to pass the Joypad port number
 */
static void joypad_transfer_pak_load_read_callback(uint64_t *out_dwords, void *ctx)
=======
    joybus_n64_accessory_write_async(
        port, JOYBUS_N64_ACCESSORY_ADDR_PROBE, write_data,
        joypad_n64_transfer_pak_enable_write_callback, (void *)port
    );
}

static void joypad_n64_transfer_pak_load_read_callback(uint64_t *out_dwords, void *ctx)
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
{
    const uint8_t *out_bytes = (void *)out_dwords;
    joypad_port_t port = (joypad_port_t)ctx;
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
<<<<<<< HEAD
    volatile joypad_transfer_pak_io_t *io = &accessory->transfer_pak_io;
    joypad_accessory_state_t state = accessory->state;
    if (!joypad_accessory_state_is_transfer_loading(state))
    {
        return; // Unexpected accessory state!
    }

    const joybus_cmd_n64_accessory_read_port_t *cmd =
        (void *)&out_bytes[port + JOYBUS_COMMAND_METADATA_SIZE];
    joybus_callback_t retry_callback = joypad_transfer_pak_load_read_callback;
    if (joypad_accessory_check_read_crc_error(port, cmd, retry_callback, ctx))
=======
    volatile joypad_n64_transfer_pak_io_t *io = &accessory->transfer_pak_io;
    joypad_accessory_state_t state = accessory->state;
    if (!joypad_accessory_state_is_transfer_loading(state)) return;

    const joybus_cmd_n64_accessory_read_port_t *recv_cmd = (void *)&out_bytes[port];
    joybus_callback_t retry_callback = joypad_n64_transfer_pak_load_read_callback;
    if (joypad_accessory_read_crc_error_check(port, recv_cmd, retry_callback, ctx))
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
    {
        return; // Accessory communication error!
    }
    else if (state == JOYPAD_ACCESSORY_STATE_TRANSFER_LOAD_STATUS_READ)
    {
<<<<<<< HEAD
        joybus_transfer_pak_status_t status = { .raw = cmd->recv.data[0] };
=======
        joybus_n64_transfer_pak_status_t status = { .raw = recv_cmd->data[0] };
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        accessory->transfer_pak_status = status;
        if (!status.access || !status.power)
        {
            // The Game Boy cartridge is no longer accessible; bail!
            accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
            accessory->error = JOYPAD_ACCESSORY_ERROR_TRANSFER_PAK_STATUS_CHANGE;
        }
        else if (io->cursor < io->end)
        {
            // Proceed with reading; select a Transfer Pak data bank
<<<<<<< HEAD
            uint8_t write_data[JOYBUS_ACCESSORY_DATA_SIZE];
=======
            uint8_t write_data[JOYBUS_N64_ACCESSORY_DATA_SIZE];
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            memset(write_data, io->bank, sizeof(write_data));
            accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_LOAD_BANK_WRITE;
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
<<<<<<< HEAD
            joybus_accessory_write_async(
                port, JOYBUS_ACCESSORY_ADDR_TRANSFER_BANK, write_data,
                joypad_transfer_pak_load_write_callback, ctx
=======
            joybus_n64_accessory_write_async(
                port, JOYBUS_N64_ACCESSORY_ADDR_TRANSFER_BANK, write_data,
                joypad_n64_transfer_pak_load_write_callback, ctx
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            );
        }
        else
        {
            // Finished reading data
            accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
        }
    }
    else if (state == JOYPAD_ACCESSORY_STATE_TRANSFER_LOAD_DATA_READ)
    {
<<<<<<< HEAD
        memcpy(io->cursor, cmd->recv.data, JOYBUS_ACCESSORY_DATA_SIZE);
        uint8_t *cursor = io->cursor += JOYBUS_ACCESSORY_DATA_SIZE;
        uint16_t tpak_addr = io->tpak_addr += JOYBUS_ACCESSORY_DATA_SIZE;
        uint16_t cart_addr = io->cart_addr += JOYBUS_ACCESSORY_DATA_SIZE;
        int next_bank = cart_addr / JOYBUS_ACCESSORY_TRANSFER_BANK_SIZE;
=======
        memcpy(io->cursor, recv_cmd->data, JOYBUS_N64_ACCESSORY_DATA_SIZE);
        uint8_t *cursor = io->cursor += JOYBUS_N64_ACCESSORY_DATA_SIZE;
        uint16_t tpak_addr = io->tpak_addr += JOYBUS_N64_ACCESSORY_DATA_SIZE;
        uint16_t cart_addr = io->cart_addr += JOYBUS_N64_ACCESSORY_DATA_SIZE;
        int next_bank = cart_addr / JOYBUS_N64_ACCESSORY_TRANSFER_BANK_SIZE;
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        if (cursor >= io->end)
        {
            // Check the Transfer Pak status again after storing:
            // If the status says reset or cart pulled, you've got a problem!
            accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_LOAD_STATUS_READ;
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
<<<<<<< HEAD
            joybus_accessory_read_async(
                port, JOYBUS_ACCESSORY_ADDR_TRANSFER_STATUS,
                joypad_transfer_pak_load_read_callback, ctx
=======
            joybus_n64_accessory_read_async(
                port, JOYBUS_N64_ACCESSORY_ADDR_TRANSFER_STATUS,
                joypad_n64_transfer_pak_load_read_callback, ctx
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            );
        }
        else if (next_bank == io->bank)
        {
            // Continue reading data
            accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_LOAD_DATA_READ;
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
<<<<<<< HEAD
            joybus_accessory_read_async(
                port, tpak_addr,
                joypad_transfer_pak_load_read_callback, ctx
=======
            joybus_n64_accessory_read_async(
                port, tpak_addr,
                joypad_n64_transfer_pak_load_read_callback, ctx
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            );
        }
        else
        {
            // Switch to the next bank
<<<<<<< HEAD
            io->tpak_addr = JOYBUS_ACCESSORY_ADDR_TRANSFER_CART;
            io->bank = next_bank;
            uint8_t write_data[JOYBUS_ACCESSORY_DATA_SIZE];
=======
            io->tpak_addr = JOYBUS_N64_ACCESSORY_ADDR_TRANSFER_CART;
            io->bank = next_bank;
            uint8_t write_data[JOYBUS_N64_ACCESSORY_DATA_SIZE];
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            memset(write_data, next_bank, sizeof(write_data));
            accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_LOAD_BANK_WRITE;
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
<<<<<<< HEAD
            joybus_accessory_write_async(
                port, JOYBUS_ACCESSORY_ADDR_TRANSFER_BANK, write_data,
                joypad_transfer_pak_load_write_callback, ctx
=======
            joybus_n64_accessory_write_async(
                port, JOYBUS_N64_ACCESSORY_ADDR_TRANSFER_BANK, write_data,
                joypad_n64_transfer_pak_load_write_callback, ctx
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            );
        }
    }
}

<<<<<<< HEAD
/**
 * @brief Callback for the accessory write commands used by #joypad_transfer_pak_load_async.
 * 
 * @param out_dwords Joybus output block
 * @param ctx Opaque pointer used to pass the Joypad port number
 */
static void joypad_transfer_pak_load_write_callback(uint64_t *out_dwords, void *ctx)
=======
static void joypad_n64_transfer_pak_load_write_callback(uint64_t *out_dwords, void *ctx)
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
{
    const uint8_t *out_bytes = (void *)out_dwords;
    joypad_port_t port = (joypad_port_t)ctx;
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
    joypad_accessory_state_t state = accessory->state;
<<<<<<< HEAD
    if (!joypad_accessory_state_is_transfer_loading(state))
    {
        return; // Unexpected accessory state!
    }

    const joybus_cmd_n64_accessory_write_port_t *cmd =
        (void *)&out_bytes[port + JOYBUS_COMMAND_METADATA_SIZE];
    joybus_callback_t retry_callback = joypad_transfer_pak_load_write_callback;
    if (joypad_accessory_check_write_crc_error(port, cmd, retry_callback, ctx))
=======
    if (!joypad_accessory_state_is_transfer_loading(state)) return;

    const joybus_cmd_n64_accessory_write_port_t *recv_cmd = (void *)&out_bytes[port];
    joybus_callback_t retry_callback = joypad_n64_transfer_pak_load_write_callback;
    if (joypad_accessory_write_crc_error_check(port, recv_cmd, retry_callback, ctx))
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
    {
        return; // Accessory communication error!
    }
    else if (state == JOYPAD_ACCESSORY_STATE_TRANSFER_LOAD_BANK_WRITE)
    {
        accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_LOAD_DATA_READ;
        accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
        accessory->retries = 0;
<<<<<<< HEAD
        joybus_accessory_read_async(
            port, accessory->transfer_pak_io.tpak_addr,
            joypad_transfer_pak_load_read_callback, ctx
=======
        joybus_n64_accessory_read_async(
            port, accessory->transfer_pak_io.tpak_addr,
            joypad_n64_transfer_pak_load_read_callback, ctx
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        );
    }
}

<<<<<<< HEAD
void joypad_transfer_pak_load_async(joypad_port_t port, uint16_t cart_addr, void *dst, size_t len)
{
    ASSERT_JOYPAD_PORT_VALID(port);
    assert(cart_addr % JOYBUS_ACCESSORY_DATA_SIZE == 0);
    assert(len % JOYBUS_ACCESSORY_DATA_SIZE == 0);
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];

    uint8_t bank = cart_addr / JOYBUS_ACCESSORY_TRANSFER_BANK_SIZE;
    uint16_t tpak_addr = JOYBUS_ACCESSORY_ADDR_TRANSFER_CART;
    tpak_addr += cart_addr % JOYBUS_ACCESSORY_TRANSFER_BANK_SIZE;
    accessory->transfer_pak_io = (joypad_transfer_pak_io_t){
=======
void joypad_n64_transfer_pak_load_async(joypad_port_t port, uint16_t cart_addr, void *dst, size_t len)
{
    ASSERT_JOYBUS_CONTROLLER_PORT_VALID(port);
    assert(cart_addr % JOYBUS_N64_ACCESSORY_DATA_SIZE == 0);
    assert(len % JOYBUS_N64_ACCESSORY_DATA_SIZE == 0);
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];

    uint8_t bank = cart_addr / JOYBUS_N64_ACCESSORY_TRANSFER_BANK_SIZE;
    uint16_t tpak_addr = JOYBUS_N64_ACCESSORY_ADDR_TRANSFER_CART;
    tpak_addr += cart_addr % JOYBUS_N64_ACCESSORY_TRANSFER_BANK_SIZE;
    accessory->transfer_pak_io = (joypad_n64_transfer_pak_io_t){
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        .start = dst,
        .end = dst + len,
        .cursor = dst,
        .bank = bank,
        .cart_addr = cart_addr,
        .tpak_addr = tpak_addr,
    };

    accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_LOAD_STATUS_READ;
    accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
    accessory->retries = 0;
<<<<<<< HEAD
    joybus_accessory_read_async(
        port, JOYBUS_ACCESSORY_ADDR_TRANSFER_STATUS,
        joypad_transfer_pak_load_read_callback, (void *)port
    );
}

/**
 * @brief Callback for the accessory read commands used by #joypad_transfer_pak_store_async.
 * 
 * @param out_dwords Joybus output block
 * @param ctx Opaque pointer used to pass the Joypad port number
 */
static void joypad_transfer_pak_store_read_callback(uint64_t *out_dwords, void *ctx)
=======
    joybus_n64_accessory_read_async(
        port, JOYBUS_N64_ACCESSORY_ADDR_TRANSFER_STATUS,
        joypad_n64_transfer_pak_load_read_callback, (void *)port
    );
}

static void joypad_n64_transfer_pak_store_read_callback(uint64_t *out_dwords, void *ctx)
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
{
    const uint8_t *out_bytes = (void *)out_dwords;
    joypad_port_t port = (joypad_port_t)ctx;
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
<<<<<<< HEAD
    volatile joypad_transfer_pak_io_t *io = &accessory->transfer_pak_io;
    joypad_accessory_state_t state = accessory->state;
    if (!joypad_accessory_state_is_transfer_storing(state))
    {
        return; // Unexpected accessory state!
    }

    const joybus_cmd_n64_accessory_read_port_t *cmd =
        (void *)&out_bytes[port + JOYBUS_COMMAND_METADATA_SIZE];
    joybus_callback_t retry_callback = joypad_transfer_pak_store_read_callback;
    if (joypad_accessory_check_read_crc_error(port, cmd, retry_callback, ctx))
=======
    volatile joypad_n64_transfer_pak_io_t *io = &accessory->transfer_pak_io;
    joypad_accessory_state_t state = accessory->state;
    if (!joypad_accessory_state_is_transfer_storing(state)) return;

    const joybus_cmd_n64_accessory_read_port_t *recv_cmd = (void *)&out_bytes[port];
    joybus_callback_t retry_callback = joypad_n64_transfer_pak_store_read_callback;
    if (joypad_accessory_read_crc_error_check(port, recv_cmd, retry_callback, ctx))
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
    {
        return; // Accessory communication error!
    }
    else if (state == JOYPAD_ACCESSORY_STATE_TRANSFER_STORE_STATUS_READ)
    {
<<<<<<< HEAD
        joybus_transfer_pak_status_t status = { .raw = cmd->recv.data[0] };
=======
        joybus_n64_transfer_pak_status_t status = { .raw = recv_cmd->data[0] };
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        accessory->transfer_pak_status = status;
        if (!status.access || !status.power)
        {
            // The Game Boy cartridge is no longer accessible; bail!
            accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
            accessory->error = JOYPAD_ACCESSORY_ERROR_TRANSFER_PAK_STATUS_CHANGE;
        }
        else if (io->cursor < io->end)
        {
            // Proceed with writing; select a Transfer Pak data bank
<<<<<<< HEAD
            uint8_t write_data[JOYBUS_ACCESSORY_DATA_SIZE];
=======
            uint8_t write_data[JOYBUS_N64_ACCESSORY_DATA_SIZE];
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            memset(write_data, io->bank, sizeof(write_data));
            accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_STORE_BANK_WRITE;
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
<<<<<<< HEAD
            joybus_accessory_write_async(
                port, JOYBUS_ACCESSORY_ADDR_TRANSFER_BANK, write_data,
                joypad_transfer_pak_store_write_callback, ctx
=======
            joybus_n64_accessory_write_async(
                port, JOYBUS_N64_ACCESSORY_ADDR_TRANSFER_BANK, write_data,
                joypad_n64_transfer_pak_store_write_callback, ctx
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            );
        }
        else
        {
            // Finished writing data
            accessory->state = JOYPAD_ACCESSORY_STATE_IDLE;
        }
    }
}

<<<<<<< HEAD
/**
 * @brief Callback for the accessory write commands used by #joypad_transfer_pak_store_async.
 * 
 * @param out_dwords Joybus output block
 * @param ctx Opaque pointer used to pass the Joypad port number
 */
static void joypad_transfer_pak_store_write_callback(uint64_t *out_dwords, void *ctx)
=======
static void joypad_n64_transfer_pak_store_write_callback(uint64_t *out_dwords, void *ctx)
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
{
    const uint8_t *out_bytes = (void *)out_dwords;
    joypad_port_t port = (joypad_port_t)ctx;
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];
<<<<<<< HEAD
    volatile joypad_transfer_pak_io_t *io = &accessory->transfer_pak_io;
    joypad_accessory_state_t state = accessory->state;
    if (!joypad_accessory_state_is_transfer_storing(state))
    {
        return; // Unexpected accessory state!
    }

    const joybus_cmd_n64_accessory_write_port_t *cmd =
        (void *)&out_bytes[port + JOYBUS_COMMAND_METADATA_SIZE];
    joybus_callback_t retry_callback = joypad_transfer_pak_store_write_callback;
    if (joypad_accessory_check_write_crc_error(port, cmd, retry_callback, ctx))
=======
    volatile joypad_n64_transfer_pak_io_t *io = &accessory->transfer_pak_io;
    joypad_accessory_state_t state = accessory->state;
    if (!joypad_accessory_state_is_transfer_storing(state)) return;

    const joybus_cmd_n64_accessory_write_port_t *recv_cmd = (void *)&out_bytes[port];
    joybus_callback_t retry_callback = joypad_n64_transfer_pak_store_write_callback;
    if (joypad_accessory_write_crc_error_check(port, recv_cmd, retry_callback, ctx))
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
    {
        return; // Accessory communication error!
    }
    else if (state == JOYPAD_ACCESSORY_STATE_TRANSFER_STORE_BANK_WRITE)
    {
        // Continue writing data
        accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_STORE_DATA_WRITE;
        accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
        accessory->retries = 0;
<<<<<<< HEAD
        joybus_accessory_write_async(
            port, io->tpak_addr, io->cursor,
            joypad_transfer_pak_store_write_callback, ctx
=======
        joybus_n64_accessory_write_async(
            port, io->tpak_addr, io->cursor,
            joypad_n64_transfer_pak_store_write_callback, ctx
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        );
    }
    else if (state == JOYPAD_ACCESSORY_STATE_TRANSFER_STORE_DATA_WRITE)
    {
<<<<<<< HEAD
        uint8_t *cursor = io->cursor += JOYBUS_ACCESSORY_DATA_SIZE;
        uint16_t tpak_addr = io->tpak_addr += JOYBUS_ACCESSORY_DATA_SIZE;
        uint16_t cart_addr = io->cart_addr += JOYBUS_ACCESSORY_DATA_SIZE;
        int next_bank = cart_addr / JOYBUS_ACCESSORY_TRANSFER_BANK_SIZE;
=======
        uint8_t *cursor = io->cursor += JOYBUS_N64_ACCESSORY_DATA_SIZE;
        uint16_t tpak_addr = io->tpak_addr += JOYBUS_N64_ACCESSORY_DATA_SIZE;
        uint16_t cart_addr = io->cart_addr += JOYBUS_N64_ACCESSORY_DATA_SIZE;
        int next_bank = cart_addr / JOYBUS_N64_ACCESSORY_TRANSFER_BANK_SIZE;
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        if (cursor >= io->end)
        {
            // Check the Transfer Pak status again after storing:
            // If the status says reset or cart pulled, you've got a problem!
            accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_STORE_STATUS_READ;
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
<<<<<<< HEAD
            joybus_accessory_read_async(
                port, JOYBUS_ACCESSORY_ADDR_TRANSFER_STATUS,
                joypad_transfer_pak_store_read_callback, ctx
=======
            joybus_n64_accessory_read_async(
                port, JOYBUS_N64_ACCESSORY_ADDR_TRANSFER_STATUS,
                joypad_n64_transfer_pak_store_read_callback, ctx
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            );
        }
        else if (next_bank == io->bank)
        {
            // Continue writing data
            accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_STORE_DATA_WRITE;
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
<<<<<<< HEAD
            joybus_accessory_write_async(
                port, tpak_addr, cursor,
                joypad_transfer_pak_store_write_callback, ctx
=======
            joybus_n64_accessory_write_async(
                port, tpak_addr, cursor,
                joypad_n64_transfer_pak_store_write_callback, ctx
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            );
        }
        else
        {
            // Switch to the next bank
<<<<<<< HEAD
            io->tpak_addr = JOYBUS_ACCESSORY_ADDR_TRANSFER_CART;
            io->bank = next_bank;
            uint8_t write_data[JOYBUS_ACCESSORY_DATA_SIZE];
=======
            io->tpak_addr = JOYBUS_N64_ACCESSORY_ADDR_TRANSFER_CART;
            io->bank = next_bank;
            uint8_t write_data[JOYBUS_N64_ACCESSORY_DATA_SIZE];
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            memset(write_data, next_bank, sizeof(write_data));
            accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_STORE_BANK_WRITE;
            accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
            accessory->retries = 0;
<<<<<<< HEAD
            joybus_accessory_write_async(
                port, JOYBUS_ACCESSORY_ADDR_TRANSFER_BANK, write_data,
                joypad_transfer_pak_store_write_callback, ctx
=======
            joybus_n64_accessory_write_async(
                port, JOYBUS_N64_ACCESSORY_ADDR_TRANSFER_BANK, write_data,
                joypad_n64_transfer_pak_store_write_callback, ctx
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
            );
        }
    }
}

<<<<<<< HEAD
void joypad_transfer_pak_store_async(joypad_port_t port, uint16_t cart_addr, void *src, size_t len)
{
    ASSERT_JOYPAD_PORT_VALID(port);
    assert(cart_addr % JOYBUS_ACCESSORY_DATA_SIZE == 0);
    assert(len % JOYBUS_ACCESSORY_DATA_SIZE == 0);
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];

    uint8_t bank = cart_addr / JOYBUS_ACCESSORY_TRANSFER_BANK_SIZE;
    uint16_t tpak_addr = JOYBUS_ACCESSORY_ADDR_TRANSFER_CART;
    tpak_addr += cart_addr % JOYBUS_ACCESSORY_TRANSFER_BANK_SIZE;
    accessory->transfer_pak_io = (joypad_transfer_pak_io_t){
=======
void joypad_n64_transfer_pak_store_async(joypad_port_t port, uint16_t cart_addr, void *src, size_t len)
{
    ASSERT_JOYBUS_CONTROLLER_PORT_VALID(port);
    assert(cart_addr % JOYBUS_N64_ACCESSORY_DATA_SIZE == 0);
    assert(len % JOYBUS_N64_ACCESSORY_DATA_SIZE == 0);
    volatile joypad_accessory_t *accessory = &joypad_accessories_hot[port];

    uint8_t bank = cart_addr / JOYBUS_N64_ACCESSORY_TRANSFER_BANK_SIZE;
    uint16_t tpak_addr = JOYBUS_N64_ACCESSORY_ADDR_TRANSFER_CART;
    tpak_addr += cart_addr % JOYBUS_N64_ACCESSORY_TRANSFER_BANK_SIZE;
    accessory->transfer_pak_io = (joypad_n64_transfer_pak_io_t){
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
        .start = src,
        .end = src + len,
        .cursor = src,
        .bank = bank,
        .cart_addr = cart_addr,
        .tpak_addr = tpak_addr,
    };

    accessory->state = JOYPAD_ACCESSORY_STATE_TRANSFER_STORE_STATUS_READ;
    accessory->error = JOYPAD_ACCESSORY_ERROR_PENDING;
    accessory->retries = 0;
<<<<<<< HEAD
    joybus_accessory_read_async(
        port, JOYBUS_ACCESSORY_ADDR_TRANSFER_STATUS,
        joypad_transfer_pak_store_read_callback, (void *)port
    );
}

/** @} */ /* joypad */
=======
    joybus_n64_accessory_read_async(
        port, JOYBUS_N64_ACCESSORY_ADDR_TRANSFER_STATUS,
        joypad_n64_transfer_pak_store_read_callback, (void *)port
    );
}
>>>>>>> 0f9be152e42aa160e217045b801f22cf11fdd5f7
