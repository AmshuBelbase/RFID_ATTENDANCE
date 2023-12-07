<?php
if (!isset($_SESSION)) {
    session_start();
}
if ((!isset($_SESSION["email"])) || (!isset($_SESSION["desig"]))) {
    echo "<script>window.location.href ='index.php';</script>";
}
// if (($_SESSION["desig"] != "ADMIN") && ($_SESSION["desig"] != "LABADMIN")) {
//     echo "<script>window.location.href ='index.php';</script>";
// }
date_default_timezone_set('Asia/Kolkata');


$email = $_SESSION['email'];
$phone = $_SESSION['phone'];
$userreg = $_SESSION['userreg'];
$desig = $_SESSION['desig'];
$username = $_SESSION['username'];
$rfid = $_SESSION['rfid'];


?>





<!doctype html>
<html lang="en">

<head>
    <link href="rel.jpg?v=<?php echo rand(); ?>" rel="icon">
    <title>Dashboard - RFID</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <link href="https://fonts.googleapis.com/css?family=Lato:300,400,700,900&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">
    <link rel="stylesheet" href="css/rfid.css">
    <link rel="stylesheet" href="css/addon.css">
    <!-- Include the jsPDF library -->
    <!-- <script src="https://unpkg.com/jspdf@latest/dist/jspdf.umd.min.js"></script> -->

    <script type="module">
        import { initializeApp } from "https://www.gstatic.com/firebasejs/10.4.0/firebase-app.js";
        import { getAuth, signInAnonymously } from "https://www.gstatic.com/firebasejs/10.4.0/firebase-auth.js";
        import {
            getDatabase,
            ref,
            set,
            child,
            equalTo,
            update,
            get,
            endAt,
            onValue,
            remove,
            query,
            onChildAdded,
            orderByChild,
        } from "https://www.gstatic.com/firebasejs/10.4.0/firebase-database.js";

        const firebaseConfig = {
            apiKey: "AIzaSyCWnDZNJqlgWq-A1OFjb6c-kYAj6IDi630",
            authDomain: "narcs-1805c.firebaseapp.com",
            databaseURL: "https://narcs-1805c.firebaseio.com",
            projectId: "narcs-1805c",
            storageBucket: "narcs-1805c.appspot.com",
            messagingSenderId: "83689842797",
            appId: "1:83689842797:web:c1cc6cf94f28b7dadf3764"
        };

        // Initialize Firebase
        const app = initializeApp(firebaseConfig);
        const auth = getAuth(app);
        const database = getDatabase(app);
        var adPhone = '<?php echo $_SESSION['phone'] ?>';
        let dynamicLabs = {};

        function countLab() {
            return new Promise((resolve, reject) => {
                const logsRef = ref(database, 'rfid/users/' + adPhone + '/labs/');

                onValue(logsRef, (snapshot) => {
                    const logsData = snapshot.val();

                    if (logsData !== null) {
                        for (const nodeKey in logsData) {
                            dynamicLabs[nodeKey] = nodeKey;
                            // console.log(dynamicLabs[nodeKey]);
                        }
                        resolve(1);
                    } else {
                        // console.log('No data found in logs');
                        resolve(1);
                    }
                });
            });
        }
        async function getCountLab() {
            try {
                await countLab();
                if (Object.keys(dynamicLabs).length <= 0) {
                    document.getElementById("waitmsg").innerHTML = "No Data to Show 👾";
                }
                // for (const key in dynamicLabs) {
                //     if (dynamicLabs.hasOwnProperty(key)) {
                //         const value = dynamicLabs[key];
                //         console.log(`Lab Code: ${key}, Lab Name: ${value}`);
                //     }
                // }
            } catch (error) {
                console.error("Error in getCountLab:", error);
            }
        }
        getCountLab();
        let giventimestamp = 86400000;
        function handleSelectChange(event) {
            giventimestamp = event.target.value;
            giventimestamp = parseInt(giventimestamp, 10);
        }

        // Get the select element
        var selectElement = document.getElementById('selectLast');
        selectElement.addEventListener('change', handleSelectChange);
        // Call getCountLab
        const options = {
            weekday: 'short',
            month: 'short',
            day: 'numeric',
            year: 'numeric',
            hour: 'numeric',
            minute: 'numeric',
            second: 'numeric',
            timeZoneName: 'short',
            hour12: true // Use 12-hour clock with AM/PM
        };
        function load_details() {
            let html = "";
            let headhtml = "";
            let rowshtml = "";
            const starCountRef = ref(database, 'rfid/logs');
            onValue(starCountRef, (snapshot) => {
                const data = snapshot.val();
                if (data != null) {
                    for (const nodeKey in data) {
                        headhtml = "";
                        if (nodeKey in dynamicLabs) {
                            if (data.hasOwnProperty(nodeKey)) {
                                // console.log(`Node Key: ${nodeKey}`);
                                const labData = ref(database, 'rfid/labs/' + nodeKey);
                                let labName = "";
                                let labname = nodeKey;
                                onValue(labData, (snapshot) => {
                                    labName = snapshot.val();
                                    labname = labName['labname'];
                                });
                                rowshtml = "";
                                headhtml += "<div class='flex_child_contain'><div class='d-flex' style='margin-top: 20px;'><div class='w-100' style='display:flex; flex:1;'><h3 class='mb-4'>Lab : " + labname + "</h3><button style='margin-left:50px; height: 50px;margin-top:-5px;' onclick='downloadExcel(\"" + nodeKey + "\")'>Download as Excel File</button></div></div>";
                                headhtml += "<div class='form-group mb-3'><div style='overflow:auto;'><table class='tbl' id='" + nodeKey + "'><tbody><tr class='tbl_row'><th class='tbl_head'>S.N</th><th class='tbl_head'>Name</th><th class='tbl_head'>Registration Number</th><th class='tbl_head'>Date</th><th class='tbl_head'>In / Out</th></tr>";
                                var sn = 1;
                                const node = data[nodeKey];
                                // Create an empty object
                                let dynamicObject = {};
                                for (const timestampKey in node) {
                                    if (node.hasOwnProperty(timestampKey)) {
                                        // console.log(`Sub Key: ${timestampKey}`);
                                        const subNode = node[timestampKey];
                                        for (const subKey in subNode) {
                                            if (subNode.hasOwnProperty(subKey)) {
                                                // console.log(`Timestamp Key: ${subKey}`);
                                                let rowhtml = "";
                                                const userData = subNode[subKey];
                                                var timestamp = (parseInt(userData.timestamp, 10) + 1695000000) * 1000;
                                                var currentDate = new Date();
                                                var currenttimestamp = currentDate.getTime();

                                                let searched_name = document.getElementById("username").value;
                                                let searched_reg = document.getElementById("userreg").value;
                                                console.log(searched_name);
                                                let nun = userData.username;
                                                let nur = userData.userreg;

                                                const date = new Date(timestamp).toLocaleString('en-US', options);
                                                let values;
                                                if (!(userData.userreg in dynamicObject)) {
                                                    dynamicObject[userData.userreg] = ["In", timestamp];
                                                    // console.log(`${userData.userreg} created : ${dynamicObject[userData.userreg]}`);
                                                }
                                                else {
                                                    values = dynamicObject[userData.userreg];
                                                    if (values[0] == "Out" || (values[0] == "In" && (timestamp - values[1]) > 43200000)) { // 12 hours - 43200000
                                                        // console.log(timestamp + " - " + values[1] + " = " + (timestamp - values[1]));
                                                        dynamicObject[userData.userreg] = ["In", timestamp];
                                                    }
                                                    else {
                                                        dynamicObject[userData.userreg] = ["Out", timestamp];
                                                    }
                                                    // console.log(`${userData.userreg} : ${dynamicObject[userData.userreg]}`);
                                                }
                                                if ((currenttimestamp - timestamp) < giventimestamp && nun.toLowerCase().includes(searched_name.toLowerCase()) && nur.toLowerCase().includes(searched_reg.toLowerCase())) {

                                                    values = dynamicObject[userData.userreg];
                                                    rowhtml += "<tr class='tbl_row'><td class='tbl_data'>";
                                                    rowhtml += sn;
                                                    rowhtml += "</td><td class='tbl_data'>";
                                                    rowhtml += userData.username;
                                                    rowhtml += "</td><td class='tbl_data'>";
                                                    rowhtml += userData.userreg;
                                                    rowhtml += "</td><td class='tbl_data'>";
                                                    rowhtml += date;
                                                    rowhtml += "</td><td class='tbl_data'>";
                                                    rowhtml += values[0];
                                                    rowhtml += "</td></tr>";
                                                    sn++;
                                                    rowshtml = rowhtml + rowshtml;
                                                }
                                                // console.log("Data:", timestampData['username']);
                                            }
                                        }
                                    }
                                }

                            }
                            html += (headhtml + rowshtml);
                            html += "</tbody></table></div></div></div>";

                            document.getElementById("tab_container").innerHTML = html;
                        }
                    }



                }
                else {
                    document.getElementById("waitmsg").innerHTML = "No Data to Show 👾";
                }
            });
        }

        setInterval(() => {
            <?php
            // if ($desig == "ADMIN" || $desig == "LABADMIN") { ?>
            load_details();
            <?php //} ?>
        }, 5000);

        <?php
        //if ($desig == "ADMIN" || $desig == "LABADMIN") { ?>
        load_details();
        <?php //} ?>

    </script>

</head>
<style>
    label,
    select,
    button {
        margin: 10px;
        padding: 8px;
        border-radius: 5px;
        font-size: 16px;
    }
</style>

<body style="padding:20px;">


    <div class="text-wrap p-4 p-lg-5 text-center d-flex align-items-center order-md-last" style="width:100%">
        <div class="text w-100">
            <h2>
                <?php echo $username; ?>

            </h2>
            <p>
                <?php echo $desig; ?>
            </p>
            <a href="./index.php" class="btn btn-white btn-outline-white" style="margin-top:10px;">Home</a>
            <?php
            if (($_SESSION["desig"] == "ADMIN") || ($_SESSION["desig"] == "LABADMIN")) {
                ?>
                <a href="./permit_lab.php" class="btn btn-white btn-outline-white" style="margin-top:10px;">Permit LAB</a>
                <a href="./add_lab.php" class="btn btn-white btn-outline-white" style="margin-top:10px;">Add Lab</a>
                <a href="./all_users.php" class="btn btn-white btn-outline-white" style="margin-top:10px;">All Users</a>

                <?php
            }
            if ($desig == "ADMIN") {
                ?>
                <a href="./allow_lab_creation.php" class="btn btn-white btn-outline-white" style="margin-top:10px;">Make LAB
                    ADMIN</a>
                <a href="./make_admin.php" class="btn btn-white btn-outline-white" style="margin-top:10px;">Make MAIN
                    ADMIN</a>
                <a href="./approve.php" class="btn btn-white btn-outline-white" style="margin-top:10px;">Approve
                    User</a>
                <a href="./remove_user.php" class="btn btn-white btn-outline-white" style="margin-top:10px;">Remove
                    User</a>
            <?php } ?>

        </div>
    </div>

    <?php
    // if ($desig == "ADMIN" || $desig == "LABADMIN") {
    ?>
    <div style='display:flex; align-content: space-between;flex-wrap: wrap;'>
        <label for="selectLast" style="margin-top: 10px; width: 120px;">Records of :</label>
        <select id="selectLast" name="selectLast" style="margin-top: 10px; width: 150px;">
            <option value="86400000">Last 24 hours </option>
            <option value="259200000">Last 3 days </option>
            <option value="604800000">Last 1 week </option>
            <option value="1296000000">Last 15 days </option>
            <option value="2592000000">Last 1 month </option>
            <option value="999999999999999">Always</option>
        </select>
        <div class="form-group mb-3" style="margin-top: 10px; width: 450px;flex: 1 1 auto;">
            <span id="userregspan" class="err"></span>
            <span id="userregspans" class="suc"></span>
            <input type="text" class="form-control" placeholder="🔍 Registration Number" id="userreg" name="userreg"
                maxlength="45" autocomplete="off" onkeyup="charOnlyreg(this)">
        </div>
        <div class="form-group mb-3" style="margin-top: 10px; width: 450px;flex: 1 1 auto;">
            <span id=" userspan" class="err"></span>
            <span id="userspans" class="suc"></span>
            <input type="text" class="form-control" placeholder="🔍 Name" id="username" name="username" maxlength="45"
                autocomplete="off" onkeyup="lettersOnly(this)">
        </div>

        <div class="tab_container" id="tab_container" style="margin-top: 10px;flex: 1 1 auto;">

            <h2 id="waitmsg">Just a sec 🔃</h2>

        </div>
    </div>
    <?php
    // } 
    // else {
    //     echo $username . " is not Allowed";
    // } 
    ?>



    </section>
    <!-- <script src="https://cdnjs.cloudflare.com/ajax/libs/jspdf/2.4.0/jspdf.umd.min.js"></script> -->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jspdf/1.5.3/jspdf.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/xlsx/0.18.5/xlsx.full.min.js"></script>
    <script>
        function downloadExcel(tableId) {
            // Get the HTML table element
            const table = document.getElementById(tableId);

            // Convert the HTML table to Excel data  
            const workbook = XLSX.utils.table_to_book(table);

            // Create a worksheet and add Excel data
            const worksheet = XLSX.utils.book_new();
            XLSX.utils.book_append_sheet(worksheet, workbook.Sheets[workbook.SheetNames[0]], 'My Data');

            // Export Excel file
            XLSX.writeFile(worksheet, (tableId + '.xlsx'));

        }
        function downloadPDF(tableId) {
            // Get the HTML table element
            const table = document.getElementById(tableId);

            //PDF PART BELOW

            // Initialize jsPDF
            var pdf = new jsPDF();
            var startY = 15;
            pdf.setFontSize(12);
            pdf.setFont("helvetica", "normal");
            var rows = table.rows;
            var headers = [];
            for (var i = 0; i < rows[0].cells.length; i++) {
                headers[i] = rows[0].cells[i].innerText.trim();
            }

            pdf.text(headers.join("   "), 15, startY);
            startY += 15; // Adjust spacing

            // Add rows to the PDF
            for (var i = 1; i < rows.length; i++) {
                var rowData = [];
                for (var j = 0; j < rows[i].cells.length; j++) {
                    rowData[j] = rows[i].cells[j].innerText.trim();
                }
                pdf.text(rowData.join("   "), 15, startY);
                startY += 10; // Adjust spacing
            }
            // Download the PDF
            pdf.save("table.pdf");

        } 
    </script>

    <script src="js/rfid.js"></script>
</body>

</html>