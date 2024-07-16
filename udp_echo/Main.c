#include <string.h>
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif


           
void _INIT CProgINIT( void )
{
    open_recv.enable = 1;
	open_recv.options = 0;
	open_recv.port = recv_to_port;
	UdpOpen( &open_recv );
	
	open_send.enable = 1;
	open_send.options = 0;
	open_send.port = send_to_port;
	UdpOpen( &open_send );
    
	recv.enable = 1;
	recv.ident = open_recv.ident;
	recv.port = recv_to_port;
	recv.pData = (UDINT) receive_buffer; 
	recv.datamax = sizeof(receive_buffer);
    
	send.pHost = (UDINT) send_to_ip;
	send.port = send_to_port;
	send.pData = (UDINT)send_buffer;
	send.datalen = sizeof(send_buffer);
	send.ident = open_send.ident;
	send.flags = 0; 
	send.enable = 1;
  
    
}


void _CYCLIC CProgCYCLIC( void )
{
	if(send.status == 65535)
	{
		send.enable = 1;	
		UdpSend( &send );
	}

	recv.enable =1;
	UdpRecv( &recv );
	if( recv.status == 0 ){
		memcpy ( send_buffer, receive_buffer, sizeof (receive_buffer) );

		send.enable = 1;	
		UdpSend( &send );

	}


}
    


void _EXIT CProgEXIT( void ){
	close_recv.enable = 1;
	close_recv.ident = open_recv.ident;
	do {
		UdpClose( &close_recv );
	}
	while( close_recv.status == 65535 );
	
	close_send.enable = 1;
	close_send.ident = close_send.ident;
	do {
		UdpClose( &close_send );
	}
	while( close_send.status == 65535 );
}