# HTTP Proxy Server

Đồ án Mạng máy tính

CHỨC NĂNG CÁC HÀM CHƯƠNG TRÌNH

string getSubStr(string input, string begin, char delim);

Chức năng của hàm :

- Lấy chuỗi con trong chuỗi (string input ) : từ chuỗi bắt đầu (string begin) đến
một kí tự kết thúc (char delim).

Kết quả khi gọi hàm :

- Trả về chuỗi con lấy được (string) thỏa điều kiện trên.


bool checkBlackList(string hostName);

Chức năng của hàm :

- Kiểm tra chuỗi đầu vào (string hostName) có nằm trong file blacklist.conf .

Kết quả khi gọi hàm :

- Trả về TRUE khi hostName truyền vào có trong blacklist.conf

- Trả về FALSE khi hostName truyền vào không có trong blacklist.conf


DWORD WINAPI handle(LPVOID lpParam);

Chức năng của hàm :

- Thực thi chương trình chính.

Kết quả khi gọi hàm :

- ExitThread(0); thoát tiến trình sau khi thực hiện xong.
