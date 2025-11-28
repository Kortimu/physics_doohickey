#include <stdio.h>
#include <windef.h>
#include <windows.h>
#include <wingdi.h>

/*

Big awesome TODO list:
- separate window for tools and whatnot
- scale up everything to not be a tiny fucking window
- maybe having the draw be SetPixel() + unrelated to the tick function is kinda dogshit
- holding button down without movement should drop sand

*/

#define FRAME_RATE 120
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

const char g_szClassName[] = "myWindowClass";
BOOL drawing = FALSE;
BOOL erasing = FALSE;
int brush_size = 2;
RGBQUAD pixels[SCREEN_WIDTH * SCREEN_HEIGHT];

void draw_pixel(HWND hwnd, COLORREF color) {
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

    RGBQUAD pixels[SCREEN_HEIGHT * SCREEN_WIDTH];

    GetDIBits(hdc_com, h_bitmap, 0, SCREEN_HEIGHT, pixels, &bmi, DIB_RGB_COLORS);

    // changing da world
    RGBQUAD pixels_next[SCREEN_HEIGHT * SCREEN_WIDTH];
    memcpy(pixels_next, pixels, SCREEN_WIDTH * SCREEN_HEIGHT * 4);

    /* we don't care about bottom row. where's it gonna move? below the window? */
    for (int i = SCREEN_WIDTH; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        if (pixels[i].rgbGreen == 255) {
            // straight down
            if (pixels[i - SCREEN_WIDTH].rgbGreen == 0) {
                pixels_next[i].rgbGreen = 0;
                pixels_next[i - SCREEN_WIDTH].rgbGreen = 255;
            }
            // both diagonal movements available
            else if (i % SCREEN_WIDTH != SCREEN_WIDTH - 1 && i % SCREEN_WIDTH != 0 && pixels[i - SCREEN_WIDTH + 1].rgbGreen == 0 && pixels[i - SCREEN_WIDTH - 1].rgbGreen == 0) {
                if (rand() % 2 == 0) {
                    // DL
                    pixels_next[i].rgbGreen = 0;
                    pixels_next[i - SCREEN_WIDTH - 1].rgbGreen = 255;
                } else {
                    // DR
                    pixels_next[i].rgbGreen = 0;
                    pixels_next[i - SCREEN_WIDTH + 1].rgbGreen = 255;
                }
            }
            // DL
            else if (i % SCREEN_WIDTH != 0 && pixels[i - SCREEN_WIDTH - 1].rgbGreen == 0) {
                pixels_next[i].rgbGreen = 0;
                pixels_next[i - SCREEN_WIDTH - 1].rgbGreen = 255;
            }
            // DR
            else if (i % SCREEN_WIDTH != SCREEN_WIDTH - 1 && pixels[i - SCREEN_WIDTH + 1].rgbGreen == 0) {
                pixels_next[i].rgbGreen = 0;
                pixels_next[i - SCREEN_WIDTH + 1].rgbGreen = 255;
            }
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
            draw_pixel(hwnd, RGB(0, 255, 0));
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
            draw_pixel(hwnd, RGB(0, 0, 0));
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
                draw_pixel(hwnd, RGB(0, 255, 0));
            } else if (erasing == TRUE) {
                draw_pixel(hwnd, RGB(0, 0, 0));
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
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
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "da sandbox",
        // WS_OVERLAPPEDWINDOW,
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
        // TODO: accomodate for other OS's
        CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH + 10, SCREEN_HEIGHT + 33,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    printf("booted skibidi rizz sigma mode");

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

        do_world_tick(hwnd);

        QueryPerformanceCounter(&now);
        double frame_time = (double)(now.QuadPart - prev.QuadPart) / (double)freq.QuadPart;

        DWORD frame_ms = (DWORD)(frame_time * 1000.0);

        if (frame_ms < target_ms) {
            Sleep(target_ms - frame_ms);
        }

        QueryPerformanceCounter(&prev);

        printf("Compute time: %lums\n", frame_ms);
    }

    return Msg.wParam;
}