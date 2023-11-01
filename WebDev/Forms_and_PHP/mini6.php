<!DOCTYPE html>
<html>
    <body>
        <?php
            $file=fopen("mini6.csv","a");
            if (!$file) {
                die("Unable to open file!");
            }
            $row = [$_POST["fname"], $_POST["lname"], $_POST["email"], $_POST["phone"], $_POST["book"], $_POST["os"]];
            fputcsv($file, $row);
            fclose($file);

            // print the lines of the csv mini6.csv
            
            $file=fopen("mini6.csv","r");
            if (!$file) {
                die("Unable to open file!");
            }
            echo "<table style='border: 1px solid black;'>";
            $even = true;
            $rows=0;
            while(! feof($file)) {
                if ($even) {
                    echo "<tr style='background-color: #77a1fc;'>";
                } else {
                    echo "<tr style='background-color: #b5caf7;'>";
                }
                $even = !$even;
                $line = fgetcsv($file);
                foreach ($line as $cell) {
                    if ($cell == "" || $cell == null) {
                        echo "<td style='border: 1px solid black;'>Entry will appear here</td>";
                    }
                    else{
                        echo "<td style='border: 1px solid black;'>" . htmlspecialchars($cell) . "</td>";
                    }
                }
                echo "</tr>";
                $rows++;
            }
            fclose($file);
            echo "</table>";
        ?>
    </body>
</html>