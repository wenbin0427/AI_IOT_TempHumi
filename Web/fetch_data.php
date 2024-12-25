<?php
    require_once('./AI_sensor.php'); // 引入資料庫連線檔案

    // 獲取最新數據的函數
    function fetchLatestData() {
        global $pdo;
        // 查詢最近的 10 條溫度數據（最新在前）
        $Sensor = $pdo->prepare("SELECT * FROM temperature_readings ORDER BY timestamp DESC LIMIT 10");
        $Sensor->execute();

        $tempData = [];
        $timestamps = [];
        $humiData=[];
        // 將查詢結果存儲在陣列中
        while ($row = $Sensor->fetch(PDO::FETCH_ASSOC)) {
            $tempData[] = $row['temperature'];  // 溫度數據
            $humiData[]= $row['humidity'];
            $timestamps[] = mb_substr($row['timestamp'], 11, 5);  // 時間戳，提取時分部分
        }

        // 將數據反轉，使其按時間從舊到新排列
        $tempData = array_reverse($tempData);
        $humiData = array_reverse($humiData);
        $timestamps = array_reverse($timestamps);
        // 返回溫度數據和時間戳數據
        return [
            'temperature' => $tempData,
            'humidity'=> $humiData,
            'timestamps' => $timestamps
        ];
    }

    // 輸出數據為 JSON 格式
    echo json_encode(fetchLatestData());
?>
