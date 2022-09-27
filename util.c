#include "Common.h"

static s8 * ___strtok=NULL;


/**************************************************************************************
*                                                                          
* 								FUNCTION DEFINITIONS(LOCAL)
*                    
***************************************************************************************/	

static void Console_WriteTxBuffers(const s8 *str, u32 length);


/*==================================================================
* Function Name  	: Console_CheckSend
* Description    	: Print a string on the HyperTerminal if we have a character in tx buffers
* Input          		: 
* Output         	: 
* Return         	: 
====================================================================*/
void Console_CheckSendData(void)
{
	UART_FIFO *p;  
	u32 tmp_wr;		     
		
	if(USART_GetFlagStatus(USART1, USART_FLAG_TXE) != RESET){
		p = &sio_tx[COM_GUI];		
		
        tmp_wr = p->write;		
		if(p->read != tmp_wr){								
			USART_SendData(USART1, p->buffer[p->read++]);				
			if(p->read >= UARTDEFS_MAX_FIFO_LEN) p->read = 0;		
		}
	}
}

/*==================================================================
* Function Name  	: COMx_CheckSendData
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void COMx_CheckSendData(u8 COMx)
{
	UART_FIFO *p;  
	u32 tmp_wr;		     

	//if(IsUseComMonitoring() && ((COMx == COM_RFU1)||(COMx == COM_RFU2))){
	if((COMx == COM_RFU1)||(COMx == COM_RFU2)){

		if(USART_GetFlagStatus(COM_PORT[COMx], USART_FLAG_TXE) != RESET){
			p = &sio_tx[COMx];						
	        tmp_wr = p->write;		
			if(p->read != tmp_wr){								
				USART_SendData(COM_PORT[COMx], p->buffer[p->read++]);				
				if(p->read >= UARTDEFS_MAX_FIFO_LEN) p->read = 0;		
			}
		}
	}
}


/*==================================================================
* Function Name  	: Console_WriteTxBuffer()
* Description    	: add string to sio_tx buffer 
* Input          		:                     
* Output         	: 
* Return         	: 
====================================================================*/
static void Console_WriteTxBuffers(const s8 *str, u32 length)
{
	u32 i;	
	u32 tmp_wr;
	UART_FIFO *p = &sio_tx[COM_GUI];	

	tmp_wr = p->write;	
	for(i = 0; i < length; i++){	
		p->buffer[tmp_wr++] = str[i];
		if(tmp_wr >= UARTDEFS_MAX_FIFO_LEN) tmp_wr = 0;
	}	

	p->write = tmp_wr;	
}



/*==================================================================
* Function Name  	: Console_WriteTxBuffer()
* Description    	: add string to sio_tx buffer 
* Input          	:                     
* Output         	: 
* Return         	: 
====================================================================*/
void COMx_Write(u8 COMx, const u8 *data, u32 length)
{
	u32 i;	
	u32 tmp_wr;
	UART_FIFO *p = &sio_tx[COMx];	

	tmp_wr = p->write;	
	for(i = 0; i < length; i++){	
		p->buffer[tmp_wr++] = data[i];
		if(tmp_wr >= UARTDEFS_MAX_FIFO_LEN) tmp_wr = 0;
	}	

	p->write = tmp_wr;	

	LED_SetTxCOM(COMx, ON);
}

/*==================================================================
* Function Name  	: EchoUart
* Description    	:
* Input          		:                     
* Output         	: 
* Return         	: 
====================================================================*/
void EchoUart(u8 nPORT)
{	
	u8	COMx = nPORT; 	
	
	u8	tmp_sio_buf[UARTDEFS_MAX_FIFO_LEN]; 	
	u8	*pBytesToParse, *src_addr; 
	u32 tmp_wr, tmp_rd, tmp_cnt, numBytesToParse;	
	
	tmp_wr	= sio_rx[COMx].write;
	tmp_rd	= sio_rx[COMx].read;
	src_addr= &sio_rx[COMx].buffer[0];	
	
	if(tmp_wr == tmp_rd) return;

	if(tmp_wr > tmp_rd){
		numBytesToParse = tmp_wr - tmp_rd;			
		memcpy((u8 *)&tmp_sio_buf[0], (u8 *)src_addr+tmp_rd, numBytesToParse);		
		
	}else{
		tmp_cnt = UARTDEFS_MAX_FIFO_LEN - tmp_rd;
		numBytesToParse =  tmp_cnt + tmp_wr;		
		memcpy((u8 *)&tmp_sio_buf[0], (u8 *)src_addr+tmp_rd, tmp_cnt);
		memcpy((u8 *)&tmp_sio_buf[tmp_cnt], (u8 *)src_addr, tmp_wr);							
		
	}
	
	pBytesToParse = &tmp_sio_buf[0];
	tmp_cnt = numBytesToParse;	

	if((COMx == COM_RFU1)||(COMx == COM_RFU2)){
		
		LED_SetRxCOM(COMx, ON);
	}
	
	UART_TxBytes(COMx, (u8 *)pBytesToParse, tmp_cnt);
	
	tmp_rd += tmp_cnt;
	if(tmp_rd >= UARTDEFS_MAX_FIFO_LEN) tmp_rd -= UARTDEFS_MAX_FIFO_LEN;
	
	sio_rx[COMx].read = tmp_rd;		
}

/*==================================================================
* Function Name  	: IsNumberString()
* Description    	: This function check the input string ascii character number is
* Input          		: s8  * str,	// is a pointer to convert
				  u32 base	//is a base value (must be in the range 2 - 16)				  
* Output         	: 
* Return         	: success : 1, fail : 0
====================================================================*/
bool IsNumberString(const s8* str, u32 base)	
{
	s8 c;
	u32 value;	
	
	if(str == 0 || *str == '\0') return (FALSE);
	
	while(*str != '\0'){
		c = *str;
		
		if (c >= '0' && c <= '9') 		value = c - '0';
		else if (c >= 'a' && c <= 'f')  value = 10 + (c -'a');
		else if (c >= 'A' && c <= 'F')  value = 10 + (c -'A');
		else{ /* a character is not an ascii number  */
			return(FALSE);
		}		
		
		if(value < base) str++;
        else return (FALSE);
	}
	
	return (TRUE);
}

/*==================================================================
* Function Name  	: IsBCD()
* Description    	: This function check the input string bcd number is
* Input          		: 
* Output         	: 
* Return         	: 
====================================================================*/
bool IsBCD(const u8 *c)
{
	u8 byte = *c;

	if(((byte & 0xF0) >> 4) < 10){
		if((byte & 0x0F) < 10) return(TRUE);
	}	
	
	return(FALSE);
}


/*==================================================================
* Function Name  	: BCD2D()
* Description    	: This function converts BCD(0-99) to Deciamal
* Input          		: 
* Output         	: 
* Return         	: 
====================================================================*/
u8 BCD2D(u8 byte)
{
  return((u8) ((((byte & 0xF0) >> 4)*10) + (byte & 0x0F)));
}


/*==================================================================
* Function Name  	: D2BCD
* Description    	: This function converts Decimal to BCD(0-99)
* Input          		: 
* Output         	: 
* Return         	: 
====================================================================*/
u8 D2BCD(u8 byte)
{
  return((u8) (((byte / 10) << 4) + (byte % 10)));
}


/*==================================================================
* Function Name  	: D2C()
* Description    	: This function converts HEX(0-F) to a ascii character
* Input          		: 
* Output         	: 
* Return         	: a character
====================================================================*/
s8 D2C(u8 c)
{
	if (c <= 9)	return '0' + c;
	
	if ((c >= 10) && (c <= 15))	return ('A' + (c - 10));

	return c;
}

/*==================================================================
* Function Name  	: C2D()
* Description    	: This function converts a ascii character('0'-'F') to HEX 
* Input          		: 
* Output         	: 
* Return         	: 
====================================================================*/
s8 C2D(s8 c)	
{
	if (c >= '0' && c <= '9') return (c - '0');
	if (c >= 'a' && c <= 'f') return (10 + (c -'a'));
	if (c >= 'A' && c <= 'F') return (10 + (c -'A'));

	return c;
}

/*==================================================================
* Function Name 	: char2asc
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void char2asc(s8 *array, u8 byte)
{	
	u8 NibbleData;
	
	/* High Nibble Data to ASCII Character */
	NibbleData = (byte >> 4 ) & 0x0F;
	array[0] = D2C(NibbleData);

	/* Low Nibble Data to ASCII Character */
	NibbleData = byte & 0x0F;
	array[1] = D2C(NibbleData);	
}



/*==================================================================
* Function Name 	: asc2bin
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
u8 asc2bin(u8 *s)
{
    u8 bin;

    bin = 0;

    if( *s >= 'A') bin = (*s -'A'+ 10 )*16;
    else  bin = (*s -'0')*16;

    if( *(s+1) >= 'A') bin = bin + *(s+1) -'A'+ 10;
    else  bin = bin + *(s+1) -'0';

    return(bin);
}

/*==================================================================
* Function Name 	: dec2str
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void dec2str(u8 dec, s8 *str)
{
    *str = (dec / 10) + '0';
    *(str+1) = (dec % 10) + '0';
}


/*==================================================================
* Function Name 	: dec1str
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void dec1str(u8 dec, s8 *str)
{
    *str = dec + '0';
}

/*==================================================================
* Function Name  	: INVBIT
* Description    	: inversion bit
* Input          		: 
* Output         	: 
* Return         	: 
====================================================================*/
u8	INVBIT(u8 byte)
{
	byte &= 0x01;
	byte ^= 0x01;
	return(byte);
}


/*==================================================================
* Function Name  	: mytoggle
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void mytoggle(u8 *byte)
{
	*byte &= 0x01;
	*byte ^= 0x01;
}


/*==================================================================
* Function Name  	: INVBYTE
* Description    	: inversion Byte
* Input          		: 
* Output         	: 
* Return         	: 
====================================================================*/
u8	INVBYTE(u8 byte)
{	
	return(byte ^= 0xff);
}

/*==================================================================
* Function Name 	: strspn
* Description    	: 
* Input          		: const char *s, 		< The string to be searched 
*				  const char *accept	< The string to search for 
* Output         	: 
* Return         	: 
====================================================================*/
size_t mystrspn(const s8 *s,const s8 *accept)
{
	const s8 *p;
	const s8 *a;
	size_t count = 0;

	for (p = s; *p != '\0'; ++p) {
		for (a = accept; *a != '\0'; ++a) {
			if (*p == *a)
				break;
		}
		if (*a == '\0')
			return count;
		++count;
	}

	return count;
}

/*==================================================================
* Function Name 	: strpbrk
* Description    	: Find the first occurrence of a set of characters
* Input          		: const char * cs,	< The string to be searched
*				  const char * ct	< The characters to search for
* Output         	: 
* Return         	: 
====================================================================*/
s8 * mystrpbrk(	const s8 * cs, const s8 * ct)
{
	const s8 *sc1,*sc2;

	for( sc1 = cs; *sc1 != '\0'; ++sc1) {
		for( sc2 = ct; *sc2 != '\0'; ++sc2) {
			if (*sc1 == *sc2)
				return (s8 *) sc1;
		}
	}
	return NULL;
}


/*==================================================================
* Function Name 	: strpbrk
* Description    	: Split a string into tokens, WARNING: strtok is deprecated, use strsep instead.
* Input          		: char * s - The string to be searched
*				  const char * ct -	The characters to search for
* Output         	: 
* Return         	: 
====================================================================*/
s8 * mystrtok(s8 * s, const s8 * ct)
{
	s8 *sbegin, *send;

	sbegin  = s ? s : ___strtok;
	if (!sbegin) {
		return NULL;
	}
	sbegin += mystrspn(sbegin,ct);
	if (*sbegin == '\0') {
		___strtok = NULL;
		return( NULL );
	}
	send = mystrpbrk( sbegin, ct);
	if (send && *send != '\0')
		*send++ = '\0';
	___strtok = send;
	return (sbegin);
}

/*==================================================================
* Function Name  	: replacetochar()
* Description    	: replace the specified character in a string with new character
* Input          		: char * str, 	// pointer to be replaced 	
				  s8 oldchar, 	//old character 
				  s8 newchar	//new character 
* Output         	: 
* Return         	: 
====================================================================*/
void replacetochar(s8 * str, s8 oldchar, s8 newchar)
{
	int x;
	for (x = 0; str[x]; x++){ /* str[x] != '\0' */
		if (str[x] == oldchar) str[x] = newchar;	
	}
}

/*==================================================================
* Function Name  	: myatoi()
* Description    	: This function converts string into integer 
* Input          		: s8  * pString,	// is a pointer to convert
				  u32 base	//is a base value (must be in the range 2 - 16)				
* Output         	: 
* Return         	: success:  >=0, fail: -1
====================================================================*/
s32 myatoi(const s8* str, u32 base)	
{
	s32 num = 0;
	
	if(!IsNumberString(str, base)) return(-1);/* Error */

	while (*str !=0) num = (num * base) + C2D(*str++);	
	
	/*	Example> *str = "123", base 10
		(0  * 10) + 1 = 1;
		(1  * 10) + 2 = 12;
		(12* 10) + 3 = 123;
		return(123);

		Example> *str = "400"(0x400), base 16
		(0  * 16) + 4 = 4;
		(4  * 16) + 0 = 64;
		(64* 16) + 0 = 1024;
		return(1024);		
	*/
	return (num);	
}

/*==================================================================
* Function Name  	: myitoa()
* Description    	: This function converts a integer number to a string. 
* Input          		: u32 value,	// is a integer value to be converted 
				  s8* const pDestination,	//< is a pointer to string to be returned
				  u32 base	//< is a base value (must be in the range 2 - 16) 
* Output         	: 
* Return         	: 
====================================================================*/
void myitoa(u32 value, s8* const pDestination, u32 base)
{
	s8 c;
	s8* str = pDestination;
	s8* tstr = pDestination;

	/* Examples :  if value is 1234 */
	if(value == 0) *str++='0';
	else{		
		while(value > 0){
			*str++ = D2C((u8)(value%base));
			value /= base;
		}
	}/* :: *str = "4321" */
	
	*str-- ='\0'; /* Add a Null character to the end of string address*/
	/* ex> "4321\0"  str point to  '1'*/
	
	while(tstr < str){
		c = *tstr;		/*'4' , '3'*/
		*tstr++ = *str;	/* '1', '2'*/	
		*str-- = c;
	}/* 1st. "1324" -> 2nd"1234"*/	
}

/*==================================================================
* Function Name  	: mytoupper() /mytoupper_string()
* Description    	: converts lower-case characters to upper-case 
* Input          		: 
* Output         	: 
* Return         	: 
====================================================================*/
s8 mytoupper(s8 c)
{
  if ((c >= 'a' ) && (c <= 'z')){
    return((s8)(c - 'a' + 'A'));
  }

  return(c);
}

s8 *mytoupper_string(s8 *str)
{
  s8 *begin = str;

  for (;*str;str++) *str = mytoupper(*str);

  return(begin);
}


/*==================================================================
* Function Name  	: mytolower() /mytolower_string()
* Description    	: converts upper-case characters to lower-case 
* Input          		: 
* Output         	: 
* Return         	: 
====================================================================*/
s8 mytolower(s8 c)
{
  if ((c >= 'A' ) && (c <= 'Z')){
    return((s8)(c - 'A' + 'a'));
  }

  return(c);
}

s8 *mytolower_string(s8 *str)
{
  s8 *begin = str;

  for (;*str;str++) *str = mytolower(*str);

  return(begin);
}

/*==================================================================
* Function Name  	: mystrncmp() /mystrcmp()
* Description    	: performs case-insensitive string comparisons
* Input          		: 
* Output         	: 
* Return         	: 0: same, 1: string 1 > 2, -1: string 2 > 1
====================================================================*/
s32 mystrncmp(const s8 *s1,const s8 *s2, u32 n )
{
  	u32 i;
	s8 c1, c2;
  	for (i = 0; i < n; i++){  

	    c1 = mytoupper(s1[i]);
    	c2 = mytoupper(s2[i]);

	    if (c1 < c2) return(-1);
    	if (c1 > c2) return(1);
  	}

	return(0);
}


s32 mystrcmp(const s8 *s1, const s8 *s2)
{
  	u32 i;
	s8 c1, c2;
	
  	for (i = 0; ;i++ ){
	    c1 = mytoupper(s1[i]);
    	c2 = mytoupper(s2[i]);
		
	    if (c1 < c2) return(-1);
    	if (c1 > c2) return(1);
	    if (c1==0) break;      /* char2 is neither < or > char1, so char2     */
    	                       /* must also == 0   */
  	}

  	return(0);
}

/*==================================================================
* Function Name  	: mymemcmp()
* Description    	: performs message comparisons
* Input          		: 
* Output         	: 
* Return         	: 0: same, 1: different
====================================================================*/
bool mymemcmp(const void *s1, const void *s2, u32 n)
{   
	const u8 *su1 = (const u8 *)s1;
    const u8 *su2 = (const u8 *)s2;

    for (; 0 < n; ++su1, ++su2, --n){			
     
      if (*su1 != *su2) return (TRUE);
		  
    }

    return (FALSE);
}

/*==================================================================
* Function Name 	:  mynewcmp
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
bool mynewcmp(u8 *pOld, u8 *pNew)
{   
	if (*pOld != *pNew){
		*pOld = *pNew;	
		return (TRUE);
	}		
    
    return (FALSE);
}

/*==================================================================
* Function Name  	: mystrlen()
* Description    	: return string length 
* Input          		: 
* Output         	: 
* Return         	: 
====================================================================*/
u32 mystrlen(const s8 *str)
{
	u32 count = 0;

	while(str[count] != '\0')count++;
	
	return (count);
}


/*==================================================================
* Function Name  	: myu8strlen
* Description    	: return string length 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
u32 myu8strlen(const u8 *str)
{
	u32 count = 0;

	while(str[count] != '\0')count++;
	
	return (count);
}


/*==================================================================
* Function Name  	: mystrcat()
* Description    	: copy char s2[] to end of s1[] 
* Input          		: 
* Output         	: 
* Return         	:
====================================================================*/
s8 *mystrcat(s8 *s1, const s8 *s2)
{ 
	s8 *s;
		
	for (s = s1; *s != '\0'; ++s); 			/* find end of s1[] */
	
	for (; (*s = *s2) != '\0'; ++s, ++s2);  /* copy s2[] to end */

	return (s); /* the end of s1[]*/
//	return (s1);    /* < cf > strcat (s8 *s1, const s8 *s2):: return value is the start of s1[]*/
}

/*==================================================================
* Function Name  :
* Description    	: copy char s2[] to  s1[] 
* Input          	: 
* Output         	: 
* Return         	:
====================================================================*/
void mystrncpy(s8* s1, s8* s2, u32 numBytes)
{
	while(numBytes--){
    	*s1++ = *s2++;
  	}
}


/*==================================================================
* Function Name  	:
* Description    	: copy char s2[] to  s1[] 
* Input          		: 
* Output         	: 
* Return         	:
====================================================================*/
void mystrcpy(s8* s1, s8* s2)
{
	while(*s2 != '\0'){
    	*s1++ = *s2++;
  	}

	*s1 = '\0';
}


/*==================================================================
* Function Name  	: mymemcpy()
* Description    	: copies memory byte-wise, replaces memcpy()
* Input          		: void *pDestination, 	 // destination of bytes to copy                 
  				  const void *pSource,    //  source of bytes to copy
        			  u32  numBytes)     	 // number of bytes to copy                       
* Output         	: 
* Return         	: 
====================================================================*/
void mymemcpy(void *pDestination, const void *pSource, u32  numBytes)      
{
        u8 *pCharDestination = (u8 *) pDestination;
  const u8 *pCharSource      = (const u8 *) pSource;

  while(numBytes--){
    *pCharDestination++ = *pCharSource++;
  }
}



/*==================================================================
* Function Name  	: myprintf()
* Description    	: Retargets the C library printf function to the USART.
* Input          		:                     
* Output         	: 
* Return         	: 
====================================================================*/  
void myprintf(char *fmt, ...)
{		
	u32 i;	
	u32 tmp_wr;
	UART_FIFO *p = &sio_tx[COM_GUI];
	char etxt[256];		
	va_list arg_ptr;
	
	va_start(arg_ptr, fmt);	vsprintf(etxt, fmt, arg_ptr);	va_end(arg_ptr);	
	
	tmp_wr = p->write;	
	for(i = 0; etxt[i] != '\0' ; i++){	
		p->buffer[tmp_wr++] = etxt[i];
		if(tmp_wr >= UARTDEFS_MAX_FIFO_LEN) tmp_wr = 0;
	}	

	p->write = tmp_wr;		
}


/* -----------------------"the examples of myprintf() usage"------------------------------
	myprintf("%#10x", 0xff);	//           0xff
	
	myprintf("%#010x", 0xff);	// 0x000000ff
	
	myprintf("%#x", 0xff)	       // 0xff
	
	%#x --> 0x	
-----------------------------------------------------------------------------------*/

/*==================================================================
* Function Name  	: myputs()
* Description    	: Print a string on the HyperTerminal
* Input          		:                     
* Output         	: 
* Return         	: 
====================================================================*/ 
void myputs(const s8 *s)
{
	Console_WriteTxBuffers(s, mystrlen(s));
}

/*==================================================================
* Function Name  	: myputsn()
* Description    	: Print a string on the HyperTerminal 
* Input          		:                     
* Output         	: 
* Return         	: 
====================================================================*/ 
void myputsn(const s8 *s, u32 n)
{
	Console_WriteTxBuffers(s, n);
}

/*==================================================================
* Function Name  	: myputsln()
* Description    	: Print a string on the HyperTerminal with CR-LF
* Input          		:                     
* Output         	: 
* Return         	: 
====================================================================*/ 
void myputsln(const s8 *s)
{
	s8 CR_LF[2] = {0x0D, 0x0A};  
//	s8* CR_LF = "\r\n"; // \r\n  0x0D, 0x0A
	Console_WriteTxBuffers(s, mystrlen(s));
	Console_WriteTxBuffers(CR_LF, 2);	
}

/*==================================================================
* Function Name  	: myputs()
* Description    	: Print a string on the HyperTerminal
* Input          		:                     
* Output         	: 
* Return         	: 
====================================================================*/ 
void myputchar(s8 c)
{
	Console_WriteTxBuffers(&c, 1);
}

/*==================================================================
* Function Name  : myputxx()
* Description    	: Print a string on the HyperTerminal
* Input          	:                     
* Output         	: 
* Return         	: 
====================================================================*/ 
void myputxx(u8 c)
{
	s8 str[3];	
	
	str[0] = D2C(c/16);
	str[1] = D2C(c%16);
	str[2] = 0x20;	/* space */
	myputsn(&str[0], 3);	
}

/*==================================================================
* Function Name  : myputxln()
* Description    	: Print a string on the HyperTerminal
* Input          	:                     
* Output         	: 
* Return         	: 
====================================================================*/ 
void myputxln(u8 c)
{
	s8 str[4];		
	
	str[0] = D2C(c/16);
	str[1] = D2C(c%16);
	str[2] = 0x0D;
	str[3] = 0x0A;
	
	myputsn(&str[0], 4);	
}


/*==================================================================
* Function Name  	: swap_u16(), swap(u32)
* Description    	: swap data
* Input          		: 
* Output         	: 
* Return         	: 
====================================================================*/
u16 swap_u16(u16 value)
{
	u16 rawData = 0;
	rawData = (value & 0xFF) << 8;
	rawData |= ((value >> 8)& 0xFF);
	return rawData;	
}

u32 swap_u32(u32 value)
{	
	u32 rawData = 0;
	rawData = (value & 0xFF) << 24;
	rawData |= ((value >> 8) & 0xFF) << 16;
	rawData |= ((value >> 16) & 0xFF) << 8;
	rawData |= ((value >> 24) & 0xFF);
	return rawData;
	/*12 34 -> 34 12 */
}


/*==================================================================
* Function Name 	: Check32bitsCheckSum
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
bool Check32bitsCheckSum(u8 *s, u32 n)
{
    u32 i;
    u32 checksum = 0, calc_sum = 0;
    u8 tempArray[4];
    
    for(i = 1; i < (n-5); i++){
        checksum += *(s+i);
    }
    
    tempArray[0] = *(s+n-5); 
    tempArray[1] = *(s+n-4); 
    tempArray[2] = *(s+n-3); 
    tempArray[3] = *(s+n-2); 

    calc_sum = (u32)(asc2bin(&tempArray[0]) << 8) & 0xFF00;
    calc_sum |= ((u32)asc2bin(&tempArray[2]) & 0X00FF);

    if( checksum == calc_sum )  return(TRUE);
    else                        return(FALSE);
        
}


/*==================================================================
* Function Name 	: GetXorCheckSum
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
u8 GetXorCheckSum(u8 *buf, u32 n)
{
	u8 CheckSum = 0;
	u32 i;

	for(i = 0; i < n; i++)CheckSum ^= buf[i];	
	
	return (CheckSum);
}



/*==================================================================
* Function Name  	: __SET_VER
* Description    	: 
* Input          		: 
* Output         	: 
* Return         	: 
====================================================================*/
bool __SET_VER(u8 *pBuffer)
{
	bool bFlag = TRUE;
	
	u32 i;
	s32 tnum;	
	s8 tsrc[10] = FW_VER_STR;
	s8 *tok = tsrc;
	
	for(i=0; i< 3; i++){

		tok = mystrtok(tok,".");
		if(!tok){
			bFlag = FALSE; break;
		}	
		
		tnum = myatoi(tok, 10);	
		
		if(tnum >= 0)pBuffer[i] = (u8)(tnum);		
		else bFlag = FALSE;
			
		tok = NULL;
	}

	return(bFlag);
}


/*==================================================================
* Function Name  	: __SET_RELEASE_DATE
* Description    	: 
* Input          		: 
* Output         	: 
* Return         	: 
====================================================================*/
bool __SET_RELEASE_DATE(u8 *pBuffer)
{

	bool bFlag = TRUE;
	
	u32 i;
	s32 tnum;	
	s8 tsrc[11] = FW_DATE_STR;
	s8 *tok = tsrc;
	
	for(i=0; i< 3; i++){

		tok = mystrtok(tok,".");
		if(!tok){
			bFlag = FALSE; break;
		}	
		
		tnum = myatoi(tok, 10);	
		
		if(tnum >= 0){

			if(i == 0){
				if(tnum >= 2000)	tnum -= 2000;
				else{
				
					bFlag = FALSE; break;
				}
			}	
			
			pBuffer[i] = (u8)(tnum);
		}
		else bFlag = FALSE;
			
		tok = NULL;
	}

	return(bFlag);
}		
