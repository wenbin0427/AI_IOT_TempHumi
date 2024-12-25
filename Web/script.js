// 取得溫度計的填充層和顯示溫度的元素
const fill = document.getElementById('fill');
const temperatureLabel = document.getElementById('temperature');
const humi_fill = document.getElementById('humi_fill');
const humi_label = document.getElementById('humi_label');


// 連接MQTT
var client = mqtt.connect('ws://iotsvm.shop:8083/'); // 這是WebSocket端點
client.on('connect', function () {
    console.log('連接至MQTT伺服器');
    client.subscribe('test/temp', function (err) {
        if (err) {
            console.log('連接失敗');
        }
    });
   
});

let latestTempData = null; // 存放最新一筆數據
let latestHumiData = null; // 存放最新一筆數據

// 訂閱取得最新數據
client.on("message", (topic, message) => {
    try {
        // 解析 JSON
        const data = JSON.parse(message.toString());
        console.log("解析後的JSON:", data.temperature);
        latestTempData = data.temperature; // 更新最新的溫度數據
        latestHumiData = data.humidity;    // 更新最新的溼度數據
    } catch (error) {
        console.error("JSON 解析失敗:", error);
    }
});

// 更新溫度計
setInterval(() => {
    if (latestTempData !== null && latestHumiData!=null) {
        // 更新溫度顯示
        temperatureLabel.textContent = `${latestTempData}°C`;
        humi_label.textContent = `${latestHumiData}%`
        // 計算高度
        const height = (latestTempData / 50) * 100;
        const humi_height = Math.min(((latestHumiData - 20) / (90 - 20)) * 100, 100);


        // 更新溫度計算高度
        fill.style.height = `${height}%`;
        humi_fill.style.height = `${humi_height}%`;

        // 根據溫度設置範圍顯示的顏色
        if (latestTempData >= 35) { //溫度超過35度
            $('#fill').removeClass('bg-danger bg-success').addClass('bg-danger');
        } else if (latestTempData >= 20 && latestTempData <35) {//溫度超過20度
            $('#fill').removeClass('bg-danger bg-primary').addClass('bg-success');
        } else if (latestTempData < 20) { //溫度低於10度
            $('#fill').removeClass('bg-success bg-danger').addClass('bg-primary');
        }

        // 根據濕度設置範圍顯示的顏色
        if (latestHumiData >= 70) {
            $('#humi_fill').removeClass('bg-danger bg-success').addClass('bg-danger');
        } else if (latestHumiData >= 60 && latestHumiData < 70) {
            $('#humi_fill').removeClass('bg-success bg-danger').addClass('bg-success');
        } else if (latestHumiData < 60) {
            $('#humi_fill').removeClass('bg-success bg-primary').addClass('bg-primary');
        }
    }
}, 2000); // 每2秒执行一次
