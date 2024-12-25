var echarts_temp = echarts.init(document.getElementById('echarts_temp'));
var echarts_humi = echarts.init(document.getElementById('echarts_humi'));

// 溫度圖表
function fetchTempData() {
    $.ajax({
        url: './fetch_data.php',
        type: 'GET',
        dataType: 'json',
        success: function (data) {
            var temperatures = data.temperature;
            var timestamps = data.timestamps;

            var option = {
                tooltip: {
                    trigger: 'axis',
                    position: function (pt) {
                        return [pt[0], '10%',];
                    }
                },
                
                title: {
                    left: 'center',
                    text: '溫度',
                },
                xAxis: {
                    type: 'category',
                    data: timestamps,
                },
                yAxis: {
                    type: 'value',
                    axisLabel: {
                        formatter: '{value}°C',
                        align:'right',
                    }
                },
                series: [
                    {
                        name:'溫度',
                        data: temperatures, // Use dynamic temperature data
                        type: 'line',
                        smooth: true,
                        lineStyle: {
                            color: 'green', // 設定線條顏色 (橘紅色)
                            width: 2         // 可選：設定線條寬度
                        },        
                        itemStyle: {
                            color: 'green'  // 可選：設定數據點顏色
                        }
                    },
                    
                    
                ]
            };
            echarts_temp.setOption(option);
        }
    });
}

// 濕度圖表
function fetchHumiData() {
    $.ajax({
        url: './fetch_data.php',
        type: 'GET',
        dataType: 'json',
        success: function (data) {
            var humidity = data.humidity;
            var timestamps = data.timestamps;

            var option = {
                tooltip: {
                    trigger: 'axis',
                    position: function (pt) {
                        return [pt[0],'10%',];
                    }
                },
                title: {
                    left: 'center',
                    text: '濕度'
                },
                xAxis: {
                    type: 'category',
                    data: timestamps,
                },
                yAxis: {
                    type: 'value',
                    axisLabel: {
                        formatter: '{value}%'
                      }
                },
                series: [
                    {
                        name:'濕度',
                        data: humidity, // Use dynamic temperature data
                        type: 'line',
                        smooth: true
                    }
                ]
            };
            echarts_humi.setOption(option);
        }
    });
}


fetchTempData();
fetchHumiData();
setInterval(fetchTempData, 1000);
setInterval(fetchHumiData, 1000);
