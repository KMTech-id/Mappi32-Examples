#ifndef WIFI_PAGE_H
#define WIFI_PAGE_H

// Menyimpan string di PROGMEM (memori program) untuk menghemat RAM
const char wifiFormHtml[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>WiFi Config</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body{font-family:Arial,sans-serif;background-color:#f0f2f5;margin:0;padding:20px;display:flex;justify-content:center;align-items:center;height:100vh}
        .container{background:#fff;padding:25px 30px;border-radius:10px;box-shadow:0 5px 15px rgba(0,0,0,0.1);width:100%;max-width:400px}
        h2{text-align:center;color:#333;margin-bottom:20px}
        label{display:block;margin-bottom:8px;color:#555;font-weight:bold}
        input[type="text"],input[type="password"]{width:100%;padding:12px;margin-bottom:20px;border-radius:5px;border:1px solid #ccc;box-sizing:border-box}
        input[type="submit"]{width:100%;background:linear-gradient(90deg, #4CAF50, #81C784);color:white;padding:14px;border:none;border-radius:5px;cursor:pointer;font-size:16px;transition:opacity 0.2s}
        input[type="submit"]:hover{opacity:0.9}
    </style>
</head>
<body>
    <div class="container">
        <h2>Konfigurasi WiFi</h2>
        <form action="/save_wifi" method="POST">
            <label for="ssid">Nama WiFi (SSID):</label>
            <input type="text" id="ssid" name="ssid" required>
            <label for="password">Password:</label>
            <input type="password" id="password" name="password">
            <input type="submit" value="Simpan & Reboot">
        </form>
    </div>
</body>
</html>
)rawliteral";

#endif // WIFI_PAGE_H