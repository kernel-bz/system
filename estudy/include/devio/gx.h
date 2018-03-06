#ifndef _GX_H_
#define _GX_H_

#define  GXERR_NONE              0                                               // 에러 없음
#define  GXERR_NO_DEVICE         1                                               // 장치가 없음
#define  GXERR_ACCESS_DEVICE     2                                               // 접근 권한이 없음
#define  GXERR_VSCREEN_INFO      3                                               // FBIOGET_VSCREENINFO를 구하지 못함
#define  GXERR_FSCREEN_INFO      4                                               // FBIOGET_FSCREENINFO를 구하지 못함
#define  GXERR_MEMORY_MAPPING    5                                               // 프레임 버퍼 메모리 매핑에 실패
#define  GXERR_NOFILE            6                                               // 이미지 파일이 없음
#define  GXERR_HEADER_INFO       7                                               // 이미지 헤더 정보 이상
#define  GXERR_READ_FILE         8                                               // 이미지 데이터 읽기 이상
#define  GXERR_PALETTE_INFO      9                                               // 팔레트 정보 이상
#define  GXERR_COLOR_DEPTH       10                                              // 칼라 깊이가 표준이 아님
#define  GXERR_NO_FONT_FILE      11                                              // 폰트파일이 없거나 사용할 수 없음
#define  GXERR_SIGNATURE         12                                              // 시그네쳐 에러
#define  GXERR_OUT_OF_MEMORY     13                                              // 메모리 부족
#define  GXERR_PROCESSING        14                                              // 처리 중 에러 발생

#define  DCTYPE_SCREEN           1                                               // DC Type 중 Screen DC 정의
#define  DCTYPE_BMP              2                                               // DC Type 중 Bitmap 정의
#define  DCTYPE_PNG              3                                               // DC Type 중 PNG 정의
#define  DCTYPE_JPG              4                                               // DC Type 중 JPG 정의


typedef struct color_t_ color_t;
struct color_t_
{
   unsigned char  red;
   unsigned char  green;
   unsigned char  blue;
   unsigned char  alpha;
};

typedef struct frame_buffer_t_ frame_buffer_t;
struct frame_buffer_t_
{
   int      fd;                                                                  // 프레임 버퍼에 대한 파일 디스크립터
   int      width;                                                               // 도트 단위의 폭
   int      height;                                                              // 도트 단위의 높이
   int      dots;                                                                // 전체 도트 갯수 width * height
   int      bytes;                                                               // 메모리 전체 크기
   int      colors;                                                              // 칼라 깊이, 1=1bit, 4=4bit, 8=8bit, ...
   int      bytes_per_line;                                                      // 라인당 바이트 개수
   int      bits_per_pixel;                                                      // 비트당 픽셀 개수

   unsigned short *mapped;                                                       // 메모리 매핑된 포인터
};

typedef struct dc_t_ dc_t;
struct dc_t_
{
   char     dc_type;                                                             // DC의 형태로 Screen, Bitmap을 구분한다.
   int      width;                                                               // 도트 단위의 폭
   int      height;                                                              // 도트 단위의 높이
   int      dots;                                                                // 전체 도트 갯수 width * height
   int      bytes;                                                               // 메모리의 전체 Byte 크기
   int      colors;                                                              // 칼라 깊이
   int      bytes_per_line;                                                      // 라인당 바이트 개수
   int      bits_per_pixel;                                                      // 비트당 픽셀 개수
   int      coor_x;                                                              // 이전에 그리기 했던 마지막 좌표
   int      coor_y;                                                              // 이전에 그리기 했던 마지막 좌표
   color_t  pen_color;                                                           // 현재의 펜 칼라
   color_t  brush_color;                                                         // 현재의 브러쉬 칼라
   void    *mapped;                                                              // 메모리 매핑된 포인터

   void (*release_dc)( dc_t *dc);                                                // Device Context 소멸 및 관련 메모리를 삭제
   void (*clear    )( dc_t *dc, color_t color);                                  // 색으로 전체 칠하기
   void (*get_pixel)( dc_t *dc, int coor_x, int coor_y, color_t   *color );      // 칼라 값을 읽어 오기
   void (*set_pixel)( dc_t *dc, int coor_x, int coor_y, color_t    color );      // 점 찍기
   void (*hline    )( dc_t *dc, int x1st  , int x_2nd , int coor_y, color_t color);// 수평선 긋기
   void (*vline    )( dc_t *dc, int coor_x, int y_1st , int y_2nd , color_t color);// 수직선 긋기
};

extern         frame_buffer_t  gx_fb;                                            // 프레임 버퍼 정보
extern         int             gx_error_code;                                    // 에러 코드

extern int     gx_init         ( char *dev_name);                                // 그래픽 라이브러리 초기화
extern void    gx_close        ( void);                                          // 그래픽 라이브러리 사용 종료
extern dc_t   *gx_get_screen_dc( void);                                          // 화면 dc를 구함
extern dc_t   *gx_get_buffer_dc( int width, int height);                         // 화면 출력을 위한 버퍼 DC를 구함
extern void    gx_release_dc   ( dc_t *dc);                                      // dc 를 소멸

extern color_t gx_color       ( int red, int green, int blue, int alpha);        // 칼라 값을 구함
extern void    gx_clear       ( dc_t *dc, color_t color);                        // 특정 색으로 전체 칠함
extern void    gx_pen_color   ( dc_t *dc, color_t color);                        // pen 칼라를 지정
extern void    gx_brush_color ( dc_t *dc, color_t color);                        // brush 칼라를 지정
extern void    gx_set_alpha   ( color_t *color, int alpha);                      // 투명도를 지정
extern void    gx_get_pixel   ( dc_t *dc, int coor_x, int coor_y, color_t *color);// 칼라 값을 읽어 오기
extern void    gx_set_pixel   ( dc_t *dc, int coor_x, int coor_y, color_t  color);// dc에 점을 찍음
extern void    gx_move_to     ( dc_t *dc, int coor_x, int coor_y);               // dc의 좌표를 이동
extern void    gx_line_to     ( dc_t *dc, int coor_x, int coor_y);               // dc의 좌표에 새 좌표까지 선을 그림
extern void    gx_line        ( dc_t *dc, int x1, int y1, int x2, int y2);       // dc에 선을 그림
extern void    gx_hline       ( dc_t *dc, int x_1st , int x_2nd , int coor_y, color_t color);// 수평선 긋기
extern void    gx_vline       ( dc_t *dc, int coor_x, int y_1st , int y_2nd , color_t color);// 수직선 긋기
extern void    gx_rectangle   ( dc_t *dc, int x1, int y1, int x2, int y2);       // dc에 사각형을 그림
extern void    gx_circle      ( dc_t *dc, int center_x, int center_y, int radius);// 원을 출력
extern void    gx_ellipse     ( dc_t *dc, int center_x, int center_y,            // 타원을 출력
                                          int width,  int height);
extern void    gx_bitblt      ( dc_t *dc_dest, int dest_x, int dest_y,           // dc 영역에 다른 dc 내용을 복사
                                dc_t *dc_sour, int sour_x, int sour_y, int sour_w, int sour_h);
extern void    gx_to_screen_dc( dc_t *dc_screen, dc_t *dc_buffer);               // DCTYPE_SCREEN dc에 다른 DCTYPE_SCREEN dc를 전체 복사
extern char   *gx_error_string( void);                                           // 에러 코드에 대한 문자열을 구함
extern int     gx_print_error ( char *remark);

//추가
extern void		gx_box (dc_t *dc, int x1, int y1, int x2, int y2);				//채움없이 사각형 그림

#endif
