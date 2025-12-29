#include <stdio.h>
#include <windef.h>
#include <windows.h>
#include <wingdi.h>
#include <wincodec.h>

#define FRAME_RATE 120
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

const char g_szClassNameMain[] = "mainWindowClass";
const char g_szClassNameRemote[] = "remoteWindowClass";
HWND hwnd_screen;
HWND hwnd_remote;

COLORREF rainbow_table[256] = {
    RGB(255,0,0), RGB(255,5,0), RGB(255,11,0), RGB(255,17,0),
    RGB(255,23,0), RGB(255,29,0), RGB(255,35,0), RGB(255,41,0),
    RGB(255,47,0), RGB(255,53,0), RGB(255,59,0), RGB(255,65,0),
    RGB(255,71,0), RGB(255,77,0), RGB(255,83,0), RGB(255,89,0),
    RGB(255,95,0), RGB(255,101,0), RGB(255,107,0), RGB(255,113,0),
    RGB(255,119,0), RGB(255,125,0), RGB(255,131,0), RGB(255,137,0),
    RGB(255,143,0), RGB(255,149,0), RGB(255,155,0), RGB(255,161,0),
    RGB(255,167,0), RGB(255,173,0), RGB(255,179,0), RGB(255,185,0),
    RGB(255,191,0), RGB(255,197,0), RGB(255,203,0), RGB(255,209,0),
    RGB(255,215,0), RGB(255,221,0), RGB(255,227,0), RGB(255,233,0),
    RGB(255,239,0), RGB(255,245,0), RGB(255,251,0), RGB(252,255,0),
    RGB(246,255,0), RGB(240,255,0), RGB(234,255,0), RGB(228,255,0),
    RGB(222,255,0), RGB(216,255,0), RGB(210,255,0), RGB(204,255,0),
    RGB(198,255,0), RGB(192,255,0), RGB(186,255,0), RGB(180,255,0),
    RGB(174,255,0), RGB(168,255,0), RGB(162,255,0), RGB(156,255,0),
    RGB(150,255,0), RGB(144,255,0), RGB(138,255,0), RGB(132,255,0),
    RGB(126,255,0), RGB(120,255,0), RGB(114,255,0), RGB(108,255,0),
    RGB(102,255,0), RGB(96,255,0), RGB(90,255,0), RGB(84,255,0),
    RGB(78,255,0), RGB(72,255,0), RGB(66,255,0), RGB(60,255,0),
    RGB(54,255,0), RGB(48,255,0), RGB(42,255,0), RGB(36,255,0),
    RGB(30,255,0), RGB(24,255,0), RGB(18,255,0), RGB(12,255,0),
    RGB(6,255,0), RGB(0,255,0), RGB(0,255,6), RGB(0,255,12),
    RGB(0,255,18), RGB(0,255,24), RGB(0,255,30), RGB(0,255,36),
    RGB(0,255,42), RGB(0,255,48), RGB(0,255,54), RGB(0,255,60),
    RGB(0,255,66), RGB(0,255,72), RGB(0,255,78), RGB(0,255,84),
    RGB(0,255,90), RGB(0,255,96), RGB(0,255,102), RGB(0,255,108),
    RGB(0,255,114), RGB(0,255,120), RGB(0,255,126), RGB(0,255,132),
    RGB(0,255,138), RGB(0,255,144), RGB(0,255,150), RGB(0,255,156),
    RGB(0,255,162), RGB(0,255,168), RGB(0,255,174), RGB(0,255,180),
    RGB(0,255,186), RGB(0,255,192), RGB(0,255,198), RGB(0,255,204),
    RGB(0,255,210), RGB(0,255,216), RGB(0,255,222), RGB(0,255,228),
    RGB(0,255,234), RGB(0,255,240), RGB(0,255,246), RGB(0,255,252),
    RGB(0,251,255), RGB(0,245,255), RGB(0,239,255), RGB(0,233,255),
    RGB(0,227,255), RGB(0,221,255), RGB(0,215,255), RGB(0,209,255),
    RGB(0,203,255), RGB(0,197,255), RGB(0,191,255), RGB(0,185,255),
    RGB(0,179,255), RGB(0,173,255), RGB(0,167,255), RGB(0,161,255),
    RGB(0,155,255), RGB(0,149,255), RGB(0,143,255), RGB(0,137,255),
    RGB(0,131,255), RGB(0,125,255), RGB(0,119,255), RGB(0,113,255),
    RGB(0,107,255), RGB(0,101,255), RGB(0,95,255), RGB(0,89,255),
    RGB(0,83,255), RGB(0,77,255), RGB(0,71,255), RGB(0,65,255),
    RGB(0,59,255), RGB(0,53,255), RGB(0,47,255), RGB(0,41,255),
    RGB(0,35,255), RGB(0,29,255), RGB(0,23,255), RGB(0,17,255),
    RGB(0,11,255), RGB(0,5,255), RGB(0,0,255), RGB(6,0,255),
    RGB(12,0,255), RGB(18,0,255), RGB(24,0,255), RGB(30,0,255),
    RGB(36,0,255), RGB(42,0,255), RGB(48,0,255), RGB(54,0,255),
    RGB(60,0,255), RGB(66,0,255), RGB(72,0,255), RGB(78,0,255),
    RGB(84,0,255), RGB(90,0,255), RGB(96,0,255), RGB(102,0,255),
    RGB(108,0,255), RGB(114,0,255), RGB(120,0,255), RGB(126,0,255),
    RGB(132,0,255), RGB(138,0,255), RGB(144,0,255), RGB(150,0,255),
    RGB(156,0,255), RGB(162,0,255), RGB(168,0,255), RGB(174,0,255),
    RGB(180,0,255), RGB(186,0,255), RGB(192,0,255), RGB(198,0,255),
    RGB(204,0,255), RGB(210,0,255), RGB(216,0,255), RGB(222,0,255),
    RGB(228,0,255), RGB(234,0,255), RGB(240,0,255), RGB(246,0,255),
    RGB(252,0,255), RGB(255,0,251), RGB(255,0,245), RGB(255,0,239),
    RGB(255,0,233), RGB(255,0,227), RGB(255,0,221), RGB(255,0,215),
    RGB(255,0,209), RGB(255,0,203), RGB(255,0,197), RGB(255,0,191),
    RGB(255,0,185), RGB(255,0,179), RGB(255,0,173), RGB(255,0,167),
    RGB(255,0,161), RGB(255,0,155), RGB(255,0,149), RGB(255,0,143),
    RGB(255,0,137), RGB(255,0,131), RGB(255,0,125), RGB(255,0,119),
    RGB(255,0,113), RGB(255,0,107), RGB(255,0,101), RGB(255,0,95),
    RGB(255,0,89), RGB(255,0,83), RGB(255,0,77), RGB(255,0,71),
    RGB(255,0,65), RGB(255,0,59), RGB(255,0,53), RGB(255,0,47),
    RGB(255,0,41), RGB(255,0,35), RGB(255,0,29), RGB(255,0,23),
    RGB(255,0,17), RGB(255,0,11), RGB(255,0,5), RGB(255,0,0)
};
int hue = 0;

enum material {
    BACKGROUND,
    SAND,
    GAY_SAND,
    ROCK
};
enum material current_brush = SAND;
BOOL drawing = FALSE;
BOOL erasing = FALSE;
int brush_size = 2;
RGBQUAD pixels[SCREEN_WIDTH * SCREEN_HEIGHT];

COLORREF material_to_color(enum material material) {
    switch (material) {
        case 0: return RGB(0, 0, 0);
        break;
        case 1: return RGB(255, 255, 0);
        break;
        case 2: {
            hue++;
            if (hue > 255) hue = 0;
            return rainbow_table[hue];
        }
        break;
        case 3: return RGB(100, 100, 100);
        break;
    }
    return RGB(255, 255, 255);
}


void draw_pixel(HWND hwnd, COLORREF color) {
    if (hwnd == hwnd_remote) return;
    POINT p;
    GetCursorPos(&p);
    if (ScreenToClient(hwnd, &p)) {
        HDC hdc = GetDC(hwnd);

        for (int i = 0; i < 2 * brush_size - 1; i++) {
            for (int j = 0; j < 2 * brush_size - 1; j++) {
                SetPixel(hdc, p.x - (brush_size - 1) + j, p.y - (brush_size - 1) + i, color);
            }
        }
        ReleaseDC(hwnd, hdc);
    }
}

BOOL is_index_in_bounds(int index) {
    return index % SCREEN_WIDTH != SCREEN_WIDTH - 1 && index % SCREEN_WIDTH != 0;
}

BOOL check_pixel_material(RGBQUAD pixel, enum material color) {
    // return pixel.rgbRed == 0 && pixel.rgbBlue == 0 && pixel.rgbGreen == 0;
    return RGB(pixel.rgbRed, pixel.rgbGreen, pixel.rgbBlue) == material_to_color(color);
}

// TODO: probs a smarter refactor possible idk
void color_pixel(RGBQUAD* pixel, COLORREF color) {
    pixel->rgbRed = GetRValue(color);
    pixel->rgbGreen = GetGValue(color);
    pixel->rgbBlue = GetBValue(color);
}

void do_world_tick(HWND hwnd) {
    HDC hdc = GetDC(hwnd);
    HDC hdc_com = CreateCompatibleDC(hdc);
    HBITMAP h_bitmap = CreateCompatibleBitmap(hdc, SCREEN_WIDTH, SCREEN_HEIGHT);

    HGDIOBJ old_bmp = SelectObject(hdc_com, h_bitmap);

    BitBlt(hdc_com, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hdc, 0, 0, SRCCOPY);

    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = SCREEN_WIDTH;
    bmi.bmiHeader.biHeight = SCREEN_HEIGHT;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    GetDIBits(hdc_com, h_bitmap, 0, SCREEN_HEIGHT, pixels, &bmi, DIB_RGB_COLORS);

    // changing da world
    RGBQUAD pixels_next[SCREEN_HEIGHT * SCREEN_WIDTH];
    memcpy(pixels_next, pixels, SCREEN_WIDTH * SCREEN_HEIGHT * 4);

    /* we don't care about bottom row. where's it gonna move? below the window? */
    for (int i = SCREEN_WIDTH; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        if (check_pixel_material(pixels[i], BACKGROUND) || check_pixel_material(pixels[i], ROCK)) continue;

        COLORREF material_color = RGB(pixels[i].rgbRed, pixels[i].rgbGreen, pixels[i].rgbBlue);

        // straight down
        if (check_pixel_material(pixels[i - SCREEN_WIDTH], BACKGROUND)) {
            color_pixel(&pixels_next[i], material_to_color(BACKGROUND));
            color_pixel(&pixels_next[i - SCREEN_WIDTH], material_color);
        }
        // both diagonal movements available
        else if (is_index_in_bounds(i) && check_pixel_material(pixels[i - SCREEN_WIDTH + 1], BACKGROUND) && check_pixel_material(pixels[i - SCREEN_WIDTH - 1], BACKGROUND)) {
            color_pixel(&pixels_next[i], material_to_color(BACKGROUND));
            if (rand() % 2 == 0) {
                // DL
                color_pixel(&pixels_next[i - SCREEN_WIDTH - 1], material_color);
            } else {
                // DR
                color_pixel(&pixels_next[i - SCREEN_WIDTH + 1], material_color);
            }
        }
        // DL
        else if (i % SCREEN_WIDTH != 0 && check_pixel_material(pixels[i - SCREEN_WIDTH - 1], BACKGROUND)) {
            color_pixel(&pixels_next[i], material_to_color(BACKGROUND));
            color_pixel(&pixels_next[i - SCREEN_WIDTH - 1], material_color);
        }
        // DR
        else if (i % SCREEN_WIDTH != SCREEN_WIDTH - 1 && check_pixel_material(pixels[i - SCREEN_WIDTH + 1], BACKGROUND)) {
            color_pixel(&pixels_next[i], material_to_color(BACKGROUND));
            color_pixel(&pixels_next[i - SCREEN_WIDTH + 1], material_color);
        }
    }

    SetDIBitsToDevice(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, SCREEN_HEIGHT, pixels_next, &bmi, DIB_RGB_COLORS);

    SelectObject(hdc_com, old_bmp);
    DeleteObject(h_bitmap);
    DeleteDC(hdc_com);
    ReleaseDC(hwnd, hdc);
    return;
}

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_LBUTTONDOWN:
        {
            draw_pixel(hwnd, material_to_color(current_brush));
            drawing = TRUE;
            return 0;
        }
        break;
        case WM_LBUTTONUP:
        {
            drawing = FALSE;
            // read_world(hwnd);
            return 0;
        }
        break;
        case WM_RBUTTONDOWN:
        {
            draw_pixel(hwnd, material_to_color(BACKGROUND));
            erasing = TRUE;
            return 0;
        }
        break;
        case WM_RBUTTONUP:
        {
            erasing = FALSE;
            // read_world(hwnd);
            return 0;
        }
        break;
        case WM_MOUSEMOVE:
        {
            if (drawing == TRUE) {
                draw_pixel(hwnd, material_to_color(current_brush));
            } else if (erasing == TRUE) {
                draw_pixel(hwnd, material_to_color(BACKGROUND));
            }
            return 0;
        }
        break;
        case WM_MOUSEWHEEL:
        {
            int direction = GET_WHEEL_DELTA_WPARAM(wParam);
            if (direction > 0) {
                brush_size++;
            } else if (direction < 0 && brush_size > 1) {
                brush_size--;
            }
            return 0;
        }
        break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// TODO: refactor for multiple pressed buttons
// (toggles and whatnot idk)
// (also just general press behaviour for when i feel like it)
int DrawRemote(HWND hwnd, int brushX, int brushY)
{
    IWICImagingFactory *pFactory = NULL;

    IWICBitmapDecoder *pDecoderBG = NULL;
    IWICBitmapFrameDecode *pFrameBG = NULL;
    IWICFormatConverter *pConverterBG = NULL;

    IWICBitmapDecoder *pDecoderBTN = NULL;
    IWICBitmapFrameDecode *pFrameBTN = NULL;
    IWICFormatConverter *pConverterBTN = NULL;

    CoInitialize(NULL);

    HRESULT hr = CoCreateInstance(&CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
                                &IID_IWICImagingFactory, (LPVOID*)&pFactory);
    if (FAILED(hr)) return -1;

    /******* load background *******/
    hr = pFactory->lpVtbl->CreateDecoderFromFilename(
        pFactory,
        L"C:\\Users\\Meira\\Desktop\\Projects\\physics_toy\\assets\\remote.png",
        NULL,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoderBG);
    if (FAILED(hr)) return -1;

    hr = pDecoderBG->lpVtbl->GetFrame(pDecoderBG, 0, &pFrameBG);
    if (FAILED(hr)) return -1;

    // convert to how UpdateLayeredWindow likes it, premultiplied and what not
    hr = pFactory->lpVtbl->CreateFormatConverter(pFactory, &pConverterBG);
    if (FAILED(hr)) return -1;

    hr = pConverterBG->lpVtbl->Initialize(
        pConverterBG,
        (IWICBitmapSource*)pFrameBG,
        &GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        NULL,
        0.0,
        WICBitmapPaletteTypeCustom);
    if (FAILED(hr)) return -1;

    UINT widthBG, heightBG;
    pConverterBG->lpVtbl->GetSize(pConverterBG, &widthBG, &heightBG);

    UINT strideBG = widthBG * 4;
    UINT bufSizeBG = strideBG * heightBG;
    BYTE* bufferBG = (BYTE*)malloc(bufSizeBG);

    hr = pConverterBG->lpVtbl->CopyPixels(pConverterBG, NULL, strideBG, bufSizeBG, bufferBG);
    if (FAILED(hr)) return -1;

    /******* load button selector thingy *******/
    hr = pFactory->lpVtbl->CreateDecoderFromFilename(
        pFactory,
        L"C:\\Users\\Meira\\Desktop\\Projects\\physics_toy\\assets\\bumton_pressed.png",
        NULL,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoderBTN);
    if (FAILED(hr)) return -1;

    hr = pDecoderBTN->lpVtbl->GetFrame(pDecoderBTN, 0, &pFrameBTN);
    if (FAILED(hr)) return -1;

    // convert to how UpdateLayeredWindow likes it, premultiplied and what not
    hr = pFactory->lpVtbl->CreateFormatConverter(pFactory, &pConverterBTN);
    if (FAILED(hr)) return -1;

    hr = pConverterBTN->lpVtbl->Initialize(
        pConverterBTN,
        (IWICBitmapSource*)pFrameBTN,
        &GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        NULL,
        0.0,
        WICBitmapPaletteTypeCustom);
    if (FAILED(hr)) return -1;

    UINT widthBTN, heightBTN;
    pConverterBTN->lpVtbl->GetSize(pConverterBTN, &widthBTN, &heightBTN);

    UINT strideBTN = widthBTN * 4;
    UINT bufSizeBTN = strideBTN * heightBTN;
    BYTE* bufferBTN = (BYTE*)malloc(bufSizeBTN);

    hr = pConverterBTN->lpVtbl->CopyPixels(pConverterBTN, NULL, strideBTN, bufSizeBTN, bufferBTN);
    if (FAILED(hr)) return -1;

    /******* do some blit goodness (bg x button) *******/
    UINT32* dst = (UINT32*)bufferBG;
    UINT32* src = (UINT32*)bufferBTN;

    for (UINT y = 0; y < heightBTN; ++y) {
        for (UINT x = 0; x < widthBTN; ++x) {
            int dx = brushX + x;
            int dy = brushY + y;

            if (dx < 0 || dy < 0 || dx >= (int)widthBG || dy >= (int)heightBG) {
                continue;
            }

            dst[dy * widthBG + dx] = src[y * widthBTN + x];
        }
    }

    /******* other shit and cleanup *******/
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = widthBG;
    bmi.bmiHeader.biHeight      = -((LONG)heightBG); // top-down
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* pvBits = NULL;
    HDC hdcScreen = GetDC(NULL);
    HBITMAP hBmp = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0);

    memcpy(pvBits, bufferBG, bufSizeBG);

    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP old = (HBITMAP)SelectObject(hdcMem, hBmp);

    RECT rc;
    GetWindowRect(hwnd, &rc);

    SIZE sizeWindow = { widthBG, heightBG };
    POINT ptSrc     = { 0, 0 };
    POINT ptDest    = { rc.left, rc.top };

    BLENDFUNCTION blend = {0};
    blend.BlendOp             = AC_SRC_OVER;
    blend.BlendFlags          = 0;
    blend.SourceConstantAlpha = 255;
    blend.AlphaFormat         = AC_SRC_ALPHA;

    UpdateLayeredWindow(hwnd, hdcScreen, &ptDest, &sizeWindow,
                        hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);

    SelectObject(hdcMem, old);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
    free(bufferBG);
    free(bufferBTN);

    pConverterBG->lpVtbl->Release(pConverterBG);
    pFrameBG->lpVtbl->Release(pFrameBG);
    pDecoderBG->lpVtbl->Release(pDecoderBG);

    pConverterBTN->lpVtbl->Release(pConverterBTN);
    pFrameBTN->lpVtbl->Release(pFrameBTN);
    pDecoderBTN->lpVtbl->Release(pDecoderBTN);
    
    pFactory->lpVtbl->Release(pFactory);
    CoUninitialize();
    return 0;
}

HBITMAP g_hbmBackground = NULL;
HBITMAP g_hbmBackgroundMask = NULL;

LRESULT CALLBACK RemProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CREATE: {
            DrawRemote(hwnd, 50, 210);


            CreateWindowEx(
                WS_EX_CLIENTEDGE,
                "BUTTON",
                "smilts",
                WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_PUSHLIKE | WS_GROUP,
                50, 210, 60, 50,
                hwnd, (HMENU)1,
                ((LPCREATESTRUCT)lParam)->hInstance,
                NULL);

            CreateWindowEx(
                WS_EX_CLIENTEDGE,
                "BUTTON",
                "gay",
                WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_PUSHLIKE,
                115, 210, 60, 50,
                hwnd, (HMENU)2,
                ((LPCREATESTRUCT)lParam)->hInstance,
                NULL);
            
            CreateWindowEx(
                WS_EX_CLIENTEDGE,
                "BUTTON",
                "akmens",
                WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_PUSHLIKE,
                180, 210, 60, 50,
                hwnd, (HMENU)3,
                ((LPCREATESTRUCT)lParam)->hInstance,
                NULL);

            CreateWindow(
                "BUTTON",
                "skibidi rizz sigma mode",
                WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_GROUP,
                50, 270, 190, 50,
                hwnd, (HMENU)69,
                ((LPCREATESTRUCT)lParam)->hInstance,
                NULL);
            
            // by default checks off the first button (default brush)
            CheckRadioButton(hwnd, 1, 3, 1);
            return 0;
        }
        // does the magic to make title-less bar draggable. idk win32 is a weird thing
        case WM_LBUTTONDOWN:
            SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
            return 0;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case 1:
                    current_brush = SAND;
                    DrawRemote(hwnd, 50, 210);
                    break;
                case 2:
                    current_brush = GAY_SAND;
                    DrawRemote(hwnd, 115, 210);
                    break;
                case 3:
                    current_brush = ROCK;
                    DrawRemote(hwnd, 180, 210);
                    break;
                case 69:
                    printf("daba dee daba do\n");
                    break;
            }
            return 0;
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            if (g_hbmBackground) {
                DeleteObject(g_hbmBackground);
                g_hbmBackground = NULL;
            }
            if (g_hbmBackgroundMask) {
                DeleteObject(g_hbmBackgroundMask);
                g_hbmBackgroundMask = NULL;
            }
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassNameMain;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    wc.lpfnWndProc = RemProc;
    // wc.hbrBackground = CreateSolidBrush(RGB(134, 119, 95));
    wc.hbrBackground = NULL;
    wc.lpszClassName = g_szClassNameRemote;

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "(Second) Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd_screen = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassNameMain,
        "awesome fucking sandbox",
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
        // TODO: accomodate for other OS's
        CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH + 10, SCREEN_HEIGHT + 33,
        NULL, NULL, hInstance, NULL);

    if(hwnd_screen == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd_screen, nCmdShow);
    UpdateWindow(hwnd_screen);

    printf("booted skibidi rizz sigma mode\n\n");

    hwnd_remote = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_APPWINDOW,
        g_szClassNameRemote,
        "comical remote",
        WS_POPUP | WS_VISIBLE,
        450, 20, 300, 600,
        NULL, NULL, hInstance, NULL);

    if (hwnd_remote == NULL) {
        MessageBox(NULL, "(Second) Window Creation Failed!", "Error!",
        MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd_remote, nCmdShow);
    UpdateWindow(hwnd_remote);

    // Step 3: The Message Loop
    LARGE_INTEGER freq, now, prev;

    DWORD target_ms = 1000 / FRAME_RATE;
    BOOL running = TRUE;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&prev);

    while (running) {
        while (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
            if (Msg.message == WM_QUIT) {
                running = FALSE;
                break;
            }

            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }

        if (!running) break;

        // genuinely unsure if this is needed, or clever to do or not. kinda just poked around and stumbled on this code
        QueryPerformanceCounter(&now);
        double inbetween_time = (double)(now.QuadPart - prev.QuadPart) / (double)freq.QuadPart;

        do_world_tick(hwnd_screen);

        QueryPerformanceCounter(&now);
        double frame_time = (double)(now.QuadPart - prev.QuadPart) / (double)freq.QuadPart;

        DWORD frame_ms = (DWORD)(frame_time * 1000.0);

        if (frame_ms < target_ms) {
            Sleep(target_ms - frame_ms);
        }

        QueryPerformanceCounter(&prev);

        // printf("Compute time: %lums\n", frame_ms);
    }

    return Msg.wParam;
}