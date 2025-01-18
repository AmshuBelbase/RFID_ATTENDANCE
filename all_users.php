<?php
if (!isset($_SESSION)) {
    session_start();
}
if (!isset($_SESSION["email"])) {
    echo "<script>window.location.href ='index.php';</script>";
}
if ((!isset($_SESSION["email"])) || (!isset($_SESSION["desig"]))) {
    echo "<script>window.location.href ='index.php';</script>";
}
if ($_SESSION["desig"] != "ADMIN" && $_SESSION["desig"] != "LABADMIN") {
    echo "<script>window.location.href ='index.php';</script>";
}
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

    <title>All Users - RFID</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <link href="https://fonts.googleapis.com/css?family=Lato:300,400,700,900&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">
    <link rel="stylesheet" href="css/rfid.css">
    <link rel="stylesheet" href="css/addon.css">
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
            apiKey: "api key",
            authDomain: "authDomain",
            databaseURL: "databaseURL",
            projectId: "projectId",
            storageBucket: "storageBucket",
            messagingSenderId: "messagingSenderId",
            appId: "appId"
        };

        // Initialize Firebase
        const app = initializeApp(firebaseConfig);
        const auth = getAuth(app);
        const database = getDatabase(app);

        function load_details() {
            let html = "";
            const starCountRef = ref(database, 'rfid/users');

            // Fetch the data and sort it by registration number
            onValue(starCountRef, (snapshot) => {
                const data = snapshot.val();

                if (data != null) {
                    // Convert the data object into an array for sorting
                    const dataArray = Object.values(data);

                    // Sort the dataArray by registration number
                    dataArray.sort((a, b) => {
                        return a.userreg.localeCompare(b.userreg);
                    });

                    // Generate HTML
                    html += "<button style='margin:5px; height: 50px;' onclick='downloadExcel()'>Download as Excel File</button><div class='form-group mb-3'><div style='overflow:auto;'><table class='tbl' id='tbl'><tbody><tr class='tbl_row'><th class='tbl_head'>S.N</th><th class='tbl_head'>NAME</th><th class='tbl_head'>REGISTRATION NUMBER</th><th class='tbl_head'>PHONE</th><th class='tbl_head'>EMAIL</th><th class='tbl_head'>RFID</th><th class='tbl_head'>DESIGNATION</th></tr>";
                    var sn = 1;

                    dataArray.forEach((node) => {
                        // Add data to the HTML table
                        let searched_name = document.getElementById("username").value;
                        let searched_reg = document.getElementById("userreg").value;
                        console.log(searched_name);
                        let nun = node.username;
                        let nur = node.userreg;
                        if (nun.toLowerCase().includes(searched_name.toLowerCase()) && nur.toLowerCase().includes(searched_reg.toLowerCase())) {
                            html += "<tr class='tbl_row'><td class='tbl_data'>";
                            html += sn;
                            html += "</td><td class='tbl_data'>";
                            html += node.username;
                            html += "</td><td class='tbl_data'>";
                            html += node.userreg;
                            html += "</td><td class='tbl_data'>";
                            html += node.phone;
                            html += "</td><td class='tbl_data'>";
                            html += node.email;
                            html += "</td><td class='tbl_data'>";
                            html += node.rfid;
                            html += "</td><td class='tbl_data'>";
                            html += node.desig;
                            html += "</td></tr>";
                            sn++;
                        }
                    });

                    html += "</tbody></table></div></div></div>";
                    document.getElementById("tab_container").innerHTML = html;
                }
            });
        }
        setInterval(() => {
            load_details();
        }, 2000);
        load_details();

    </script>

</head>
<style>
    .tbl {
        width: 100%;
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
            <?php
            if (($_SESSION["desig"] == "ADMIN") || ($_SESSION["desig"] == "LABADMIN")) {
                ?>
                <a href="./dashboard.php" class="btn btn-white btn-outline-white" style="margin-top:10px;">Dash Board</a>
                <a href="./index.php" class="btn btn-white btn-outline-white" style="margin-top:10px;">Home</a>
                <a href="./permit_lab.php" class="btn btn-white btn-outline-white" style="margin-top:10px;">Permit LAB</a>
                <a href="./add_lab.php" class="btn btn-white btn-outline-white" style="margin-top:10px;">Add Lab</a>
            <?php }
            if ($_SESSION['desig'] == "ADMIN") {
                ?>
                <a href="./allow_lab_creation.php" class="btn btn-white btn-outline-white" style="margin-top:10px;">Make LAB
                    ADMIN</a>
                <a href="./make_admin.php" class="btn btn-white btn-outline-white" style="margin-top:10px;">Make
                    MAIN ADMIN</a>
                <a href="./approve.php" class="btn btn-white btn-outline-white" style="margin-top:10px;">Approve
                    User</a>
                <a href="./remove_user.php" class="btn btn-white btn-outline-white" style="margin-top:10px;">Remove
                    User</a>
            <?php } ?>

        </div>
    </div>

    <!-- <div class="tab_con" id="tab_con">
    </div> -->
    <div class="container" style="display:flex;align-content: space-between;flex-wrap: wrap;">
        <div class="form-group mb-3" style="margin-top: 10px; width: 50%;flex: 1 1 auto;">
            <span id="userspan" class="err"></span>
            <span id="userspans" class="suc"></span>
            <input type="text" class="form-control" placeholder="🔍 Name" id="username" name="username" maxlength="45"
                autocomplete="off" onkeyup="lettersOnly(this)">
        </div>
        <div class="form-group mb-3" style="margin-top: 10px; width: 50%;flex: 1 1 auto;">
            <span id="userregspan" class="err"></span>
            <span id="userregspans" class="suc"></span>
            <input type="text" class="form-control" placeholder="🔍 Registration Number" id="userreg" name="userreg"
                maxlength="45" autocomplete="off" onkeyup="charOnlyreg(this)">
        </div>

        <div class="tab_container" id="tab_container" style="margin-top: 10px;flex: 1 1 auto;">
            <h2 id="waitmsg">Just a sec 🔃</h2>
        </div>
    </div>




    </section>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jspdf/1.5.3/jspdf.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/xlsx/0.18.5/xlsx.full.min.js"></script>
    <script>
        function downloadExcel() {
            // Get the HTML table element
            const table = document.getElementById("tbl");

            // Convert the HTML table to Excel data  
            const workbook = XLSX.utils.table_to_book(table);

            // Create a worksheet and add Excel data
            const worksheet = XLSX.utils.book_new();
            XLSX.utils.book_append_sheet(worksheet, workbook.Sheets[workbook.SheetNames[0]], 'My Data');

            // Export Excel file
            XLSX.writeFile(worksheet, 'users.xlsx');

        }
    </script>
    <script src="js/rfid.js"></script>

</body>

</html>