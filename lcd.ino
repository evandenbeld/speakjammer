#include "ports.h"

#define UI_SCREEN_WIDTH 64
#define UI_SCREEN_HEIGHT 128

#define UI_HEADER_OFFSET 2
#define UI_HEADER_HEIGHT UI_ROW_HEIGHT + UI_HEADER_OFFSET

#define UI_ROW_HEIGHT 6
#define UI_ROW_WIDTH 16
#define UI_ROW_OFFSET UI_HEADER_HEIGHT + 2

#define UI_CELL_WIDTH UI_ROW_WIDTH - 1
#define UI_CELL_HEIGHT UI_ROW_HEIGHT + 1

void setupLCD() {
  u8g2.begin();

  u8g2.firstPage();
  do {

    u8g2.clear();

    drawCanvas();
    drawHeader();
    drawTable();

    xorChannel(0);

  } while ( u8g2.nextPage() );
}

inline void drawCanvas() {
  drawFullHorizontalLine(1);
  drawFullHorizontalLine(UI_HEADER_HEIGHT + 1);
  drawFullHorizontalLine(UI_SCREEN_HEIGHT);

  drawFullVerticalLine(15);
  drawFullVerticalLine(31);
  drawFullVerticalLine(47);
}

inline void drawFullHorizontalLine(unsigned int y)
{
  u8g2.drawLine(0, y, UI_SCREEN_WIDTH, y);
}

inline void drawFullVerticalLine(unsigned int x)
{
  u8g2.drawLine(x, UI_HEADER_HEIGHT + 1, x, UI_SCREEN_HEIGHT);
}

inline void drawHeader()
{
  u8g2.setFont(u8g2_font_sirclivethebold_tn  );
  for (int i = 0; i < NUMBER_OF_CHANNELS; i++)
  {
    u8g2.setCursor(4 + (i * UI_ROW_WIDTH), UI_HEADER_HEIGHT);
    u8g2.print(i + 1);
  }
}

inline void drawTable()
{
  u8g2.setFont(u8g2_font_tom_thumb_4x6_te  );
  for (int i = 0; i < NUMBER_OF_ROWS; i++)
  {
    for (int j = 0; j < NUMBER_OF_CHANNELS; j++)
    {
      int x = 1 + (j * UI_ROW_WIDTH);
      int y = UI_ROW_OFFSET + UI_ROW_HEIGHT + (i * UI_ROW_HEIGHT);
      u8g2.setCursor(x, y);
      u8g2.print(controlCodes[i][j]);
    }
  }
}

inline void xorChannel(unsigned int channel)
{
  do {
    u8g2.setDrawColor(2);
    byte  x = channel * UI_ROW_WIDTH;
    u8g2.drawBox(x, UI_HEADER_OFFSET, UI_CELL_WIDTH, UI_CELL_HEIGHT);
  } while ( u8g2.nextPage() );
}

inline void xorCell(unsigned int channel, unsigned int row)
{
  do {
    u8g2.setDrawColor(2);
    byte  x = (channel * UI_ROW_WIDTH);
    byte y = UI_ROW_OFFSET + (row * UI_ROW_HEIGHT);
    u8g2.drawBox(x, y, UI_CELL_WIDTH, UI_CELL_HEIGHT);
  } while ( u8g2.nextPage() );
}

inline void updateCell(unsigned int channel, unsigned int row, byte controlCode)
{
  do {
    u8g2.setDrawColor(1);
    byte  x = (channel * UI_ROW_WIDTH);
    byte y = UI_ROW_OFFSET + (row * UI_ROW_HEIGHT);
    u8g2.drawBox(x, y, UI_CELL_WIDTH, UI_CELL_HEIGHT);

    u8g2.setDrawColor(0);
    u8g2.setFont(u8g2_font_tom_thumb_4x6_te);
    u8g2.setCursor(x, y + UI_ROW_HEIGHT);
    u8g2.print(controlCode);
  } while ( u8g2.nextPage() );
}
