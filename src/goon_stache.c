// void do_world_tick(HWND hwnd) {
//     HDC hdc = GetDC(hwnd);
//     HDC hdc_com = CreateCompatibleDC(hdc);
//     // HBITMAP h_bitmap = CreateCompatibleBitmap(hdc, SCREEN_WIDTH, SCREEN_HEIGHT);

//     // SelectObject(hdc_com, h_bitmap);

//     // BitBlt(hdc_com, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hdc, 0, 0, SRCCOPY);

//     BITMAPINFO bmi = {0};
//     // ZeroMemory(&bmi, sizeof(bmi));
//     bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//     bmi.bmiHeader.biWidth = SCREEN_WIDTH;
//     bmi.bmiHeader.biHeight = SCREEN_HEIGHT;
//     bmi.bmiHeader.biPlanes = 1;
//     bmi.bmiHeader.biBitCount = 32;
//     bmi.bmiHeader.biCompression = BI_RGB;

//     void *pixel_bits = NULL;

//     HBITMAP h_bitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pixel_bits, NULL, 0);
//     HBITMAP old_bitmap = (HBITMAP)SelectObject(hdc_com, h_bitmap);

//     BitBlt(hdc_com, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hdc, 0, 0, SRCCOPY);

//     RGBQUAD *pixels = (RGBQUAD*)pixel_bits;

//     printf("done...?\n");

//     bool found = false;
//     uint32_t index = 0;
//     for (uint32_t i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
//         if (pixels[i].rgbGreen == 255 && pixels[i].rgbRed == 0 && pixels[i].rgbBlue == 0) {
//             found = true;
//             index = i;
//             break;
//         }
//     }

//     printf("green pixel found? %d (it was pixel #%d)\n", found, index);
//     printf("first pixel check: %u, %u, %u\n\n", pixels[4480].rgbRed, pixels[4480].rgbGreen, pixels[4480].rgbBlue);
//     RECT r; GetClientRect(hwnd, &r); printf("client w=%ld h=%ld\n", r.right-r.left, r.bottom-r.top);

//     return;
// }