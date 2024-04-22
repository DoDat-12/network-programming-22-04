# Network Programming 22-04 🍖
Bài tập về nhà lập trình mạng ngày 22/04
Sử dụng kỹ thuật đa tiến trình (multiprocessing) thực hiện các bài tập sau:
Bài 1: Lập trình lại ứng dụng http_server (slide 154) sử dụng kỹ thuật preforking.
Code: http_prefork.c
Bài 2: Lập trình ứng dụng time_server thực hiện chức năng sau:
+ Chấp nhận nhiều kết nối từ các client.
+ Cient gửi lệnh GET_TIME [format] để nhận thời gian từ server.
+ format là định dạng thời gian server cần trả về client. Các format cần hỗ trợ gồm:
    dd/mm/yyyy – vd: 30/01/2023
    dd/mm/yy – vd: 30/01/23
    mm/dd/yyyy – vd: 01/30/2023
    mm/dd/yy – vd: 01/30/23
+ Cần kiểm tra tính đúng đắn của lệnh client gửi lên server.
Code: time_server.c
