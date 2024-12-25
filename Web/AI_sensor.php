<?php
try{
    $pdo = new PDO(
        'mysql:host=34.81.46.57;port=3306;
        dbname=sensor_data;
        charset=utf8mb4',//連線字串，port預設為3306，若不是則新增port=XXXX
        'root',//帳號
        //CREATE user 'Text1_user'@'localhost' identified by 'Text1_password';  創建帳號密碼
        '1234',//密碼
        //GRANT all on Interactive_Database_mydb.* TO 'Text1_user'@'localhost'; 給予這組帳號 "全部"的權限
        [   //設定
            PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
            PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC 
            
        ]
        );
} catch (Exception $a) { //如果上方有錯誤則顯示錯誤訊息
    echo '連線錯誤';
    echo $a -> getMessage(PDO::FETCH_ASSOC);//拋出錯誤訊息
    exit();//跳出
}

?>
