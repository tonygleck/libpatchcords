// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#include <stddef.h>

#include "lib-util-c/sys_debug_shim.h"
#include "lib-util-c/app_logging.h"
#include "patchcords/patchcord_client.h"

typedef struct PATCH_INSTANCE_TAG
{
    const IO_INTERFACE_DESCRIPTION* io_interface_description;
    CORD_HANDLE concrete_xio_handle;
} PATCH_INSTANCE;

PATCH_INSTANCE_HANDLE patchcord_client_create(const IO_INTERFACE_DESCRIPTION* io_interface_description, const void* parameters, const PATCHCORD_CALLBACK_INFO* client_cb)
{
    PATCH_INSTANCE* result;
    if ((io_interface_description == NULL) ||
        (io_interface_description->interface_impl_create == NULL) ||
        (io_interface_description->interface_impl_destroy == NULL) ||
        (io_interface_description->interface_impl_open == NULL) ||
        (io_interface_description->interface_impl_close == NULL) ||
        (io_interface_description->interface_impl_send == NULL) ||
        (io_interface_description->interface_impl_process_item == NULL) ||
        (io_interface_description->interface_impl_query_uri == NULL) ||
        (io_interface_description->interface_impl_query_port == NULL) )
    {
        log_error("Invalid interface description specified");
        result = NULL;
    }
    else if (client_cb == NULL)
    {
        log_error("client callback parameter is NULL");
        result = NULL;
    }
    else
    {
        /* Codes_SRS_XIO_01_017: [If allocating the memory needed for the IO interface fails then xio_create shall return NULL.] */
        if ((result = (PATCH_INSTANCE*)malloc(sizeof(PATCH_INSTANCE))) != NULL)
        {
            result->io_interface_description = io_interface_description;
            if ((result->concrete_xio_handle  = result->io_interface_description->interface_impl_create(parameters, client_cb)) == NULL)
            {
                log_error("Failure calling interface create");
                free(result);
                result = NULL;
            }
        }
        else
        {
            log_error("Failure allocating io instance");
        }
    }
    return (PATCH_INSTANCE_HANDLE)result;
}

void patchcord_client_destroy(PATCH_INSTANCE_HANDLE xio)
{
    if (xio != NULL)
    {
        PATCH_INSTANCE* xio_instance = (PATCH_INSTANCE*)xio;
        xio_instance->io_interface_description->interface_impl_destroy(xio_instance->concrete_xio_handle);
        free(xio_instance);
    }
}

int patchcord_client_open(PATCH_INSTANCE_HANDLE xio, ON_IO_OPEN_COMPLETE on_io_open_complete, void* on_io_open_complete_context)
{
    int result;
    if (xio == NULL)
    {
        log_error("Invalid parameter specified");
        result = __LINE__;
    }
    else
    {
        PATCH_INSTANCE* xio_instance = (PATCH_INSTANCE*)xio;
        result = xio_instance->io_interface_description->interface_impl_open(xio_instance->concrete_xio_handle, on_io_open_complete, on_io_open_complete_context);
    }
    return result;
}

int patchcord_client_listen(PATCH_INSTANCE_HANDLE xio, ON_INCOMING_CONNECT incoming_conn, void* user_ctx)
{
    int result;
    if (xio == NULL)
    {
        log_error("Invalid parameter specified");
        result = __LINE__;
    }
    else
    {
        PATCH_INSTANCE* xio_instance = (PATCH_INSTANCE*)xio;
        if (xio_instance->io_interface_description->interface_impl_listen == NULL)
        {
            log_error("Failure listening function not implemented");
            result = __LINE__;
        }
        else
        {
            result = xio_instance->io_interface_description->interface_impl_listen(xio_instance->concrete_xio_handle, incoming_conn, user_ctx);
        }
    }
    return result;
}

int patchcord_client_close(PATCH_INSTANCE_HANDLE xio, ON_IO_CLOSE_COMPLETE on_io_close_complete, void* callback_context)
{
    int result;
    if (xio == NULL)
    {
        log_error("Invalid parameter specified");
        result = __LINE__;
    }
    else
    {
        PATCH_INSTANCE* xio_instance = (PATCH_INSTANCE*)xio;
        result = xio_instance->io_interface_description->interface_impl_close(xio_instance->concrete_xio_handle, on_io_close_complete, callback_context);
    }
    return result;
}

int patchcord_client_send(PATCH_INSTANCE_HANDLE xio, const void* buffer, size_t size, ON_SEND_COMPLETE on_send_complete, void* callback_context)
{
    int result;
    if (xio == NULL)
    {
        log_error("Invalid parameter specified");
        result = __LINE__;
    }
    else
    {
        PATCH_INSTANCE* xio_instance = (PATCH_INSTANCE*)xio;
        result = xio_instance->io_interface_description->interface_impl_send(xio_instance->concrete_xio_handle, buffer, size, on_send_complete, callback_context);
    }
    return result;
}

void patchcord_client_process_item(PATCH_INSTANCE_HANDLE xio)
{
    if (xio != NULL)
    {
        PATCH_INSTANCE* xio_instance = (PATCH_INSTANCE*)xio;
        xio_instance->io_interface_description->interface_impl_process_item(xio_instance->concrete_xio_handle);
    }
}

const char* patchcord_client_query_endpoint(PATCH_INSTANCE_HANDLE xio, uint16_t* port)
{
    const char* result;
    if (xio == NULL)
    {
        log_error("Invalid parameter specified");
        result = NULL;
    }
    else
    {
        PATCH_INSTANCE* xio_instance = (PATCH_INSTANCE*)xio;

        if (port != NULL)
        {
            *port = xio_instance->io_interface_description->interface_impl_query_port(xio_instance->concrete_xio_handle);
        }
        result = xio_instance->io_interface_description->interface_impl_query_uri(xio_instance->concrete_xio_handle);
    }
    return result;
}
