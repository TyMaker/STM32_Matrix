
STM32����RGBȫ�ʵ�����ʾ��
=======================================================
��д�ߣ� Makt ����Ƽ� ���˲���: www.makt.top 

���ܣ�
------------

 * ��ʾ24λȫ��ͼƬ ͼƬ��С32 * 32
 * ��ʾ16*16 ���ģ������ֿ⣩
	void draw_hanzi(u8 x, u8 y, u32 Color, char *hanzi);
 * ֧��4������ASCII �ַ���ʾ
	void drawChar(u8 x, u8 y, u32 Color, char character);
	void drawString(u8 x, u8 y, u32 Color, char *text);

 * ֧����ʾ��̬ASCII �ַ�����
	setScrollMode(wrapForward); //��ʾģʽ
	setScrollSpeed(25);         //��ʱ
	setScrollFont(font5x7);     //����
	setScrollColor(Color888(5, 74,74));
	setScrollbgColor(Color888(5,5,5));
	setScrollXY(3);             //��ʾλ��
	scrollText("Hello World", 1);//��ʼ��ʾ

 * ��ʾ���Ǻ���
	fsin();
	
 * ��ʾָ��ʱ��
	init_Point_Clock();
	
 * ��ʾʱ��ʪ��
 
 * ��ʾ
 
 * ����SPI Flash
 	for(i=(45870*16);i<(766090);i=i+2)  //����
	 printf("%04X ",SPI_FLASH_Read(i)); //���� ���spi flash��д��ȷ���
	 
 * ����
	ClearBuff(0,1024);
	
 * 
 
 
�������ܣ�
------------



Ӳ��������
------------

 * STM32f103CBt6
 * AT24c02 Eeprom
 * SPI Flash ����ֿ��ļ� w25Q16  2MByte
 * 2��16*32 RGB ������ʾ�� 
 * ��������
 * 5V 2A ��Դ
 
