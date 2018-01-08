<?php

    $servername = "localhost";
    $username = "temperatureUser";
    $password = "temperaturePassword16";
    $dbname = "homesensors";

    echo "post data: ";
    print_r($_POST);
    echo " <br>";
    
    if(isset($_POST['temp'])) {
        $temp = htmlentities($_POST['temp'],ENT_QUOTES, 'UTF-8');

        //make connection
        $conn=mysqli_connect($servername,$username,$password,$dbname);
        //check connection
        if($conn->connnect_error) {
            die("connection failed: " . $conn->connect_error);
        }

        $sql = "INSERT INTO temperaturereadings (id, time, temperature) VALUES (NULL, NULL,". $temp.")";
        if ($conn->query($sql) == TRUE) {
            echo "New record created successfully";
        }else{
            echo "Error: " . $sql . "<br>" . $conn->error, E_USER_ERROR;
        }
    
    }else{
        echo "Couldn't write to db lets read from it<br>";

    
    
        //crate connection
        $conn=mysqli_connect($servername,$username,$password,$dbname);
        //check connection

        if(mysqli_connect_errno()) {
            die("connection failed: " . $conn->connect_error);
        }else{
            echo "we have connected to the database.<br><br>";
        
            $queryString = "Select * from temperaturereadings";
        
            $result = $conn->query($queryString);
        
            echo "<table border='1'><tr><th>id</th><th>date</th><th>temperature</th></tr>";
            
            if($result->num_rows > 0){
                while($row = $result->fetch_assoc()) {
                    echo "<tr><td>" . $row["id"]. "</td><td>" . $row["time"]. "</td><td>" . $row["temperature"]. "</td></tr>";
                }
                echo "</table>";
            } else {
                echo "0 results";
            }
        }
    
    }
    
    $conn->close();



    
?>
