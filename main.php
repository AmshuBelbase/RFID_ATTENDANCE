<?php
if (!isset($_SESSION)) {
    session_start();
}
if (!isset($_SESSION["email"])) {
    echo "<script>window.location.href ='index.php';</script>";
}
date_default_timezone_set('Asia/Kolkata');


$email = $_SESSION['email'];
$phone = $_SESSION['phone'];
$userreg = $_SESSION['userreg'];
$desig = $_SESSION['desig'];
$username = $_SESSION['username'];
$rfid = $_SESSION['rfid'];

if (isset($_POST['log_out'])) {
    session_unset();
    session_destroy();
    // if (isset($_COOKIE["log"])) {
    //     setcookie("log", 0, time() - 604800, "/");
    // }
    echo "<script>window.location.href ='index.php';</script>";
}

?>





<!doctype html>
<html lang="en">

<head>
    <link href="rel.jpg?v=<?php echo rand(); ?>" rel="icon">
    <title>RFID MAIN</title>
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

        function load_details() {
            let html = "";
            let headhtml = "";
            let rowshtml = "";
            const starCountRef = ref(database, 'rfid/logs');
            onValue(starCountRef, (snapshot) => {
                const data = snapshot.val();
                if (data != null) {

                    for (const nodeKey in data) {
                        var htmlt = "";
                        headhtml = "";
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
                            headhtml += "<div class='flex_child_contain'><div class='d-flex' style='margin-top: 20px;'><div class='w-100'><h3 class='mb-4'>Lab : " + labname + "</h3></div></div>";
                            headhtml += "<div class='form-group mb-3'><div style='overflow:auto;'><table class='tbl'><tbody><tr class='tbl_row'><th class='tbl_head'>S.N</th><th class='tbl_head'>Date</th><th class='tbl_head'>In / Out</th></tr>";
                            var sn = 1;
                            const node = data[nodeKey];
                            let dynamicObject = {};
                            for (const timestampKey in node) {
                                if (node.hasOwnProperty(timestampKey)) {
                                    // console.log(`Sub Key: ${timestampKey}`);
                                    const subNode = node[timestampKey];
                                    for (const subKey in subNode) {
                                        if (subNode.hasOwnProperty(subKey)) {
                                            // console.log(`Timestamp Key: ${subKey}`);
                                            const userData = subNode[subKey];
                                            var rfid = "<?php echo $rfid ?>";
                                            if (rfid == userData.rf) {
                                                let rowhtml = "";
                                                htmlt = "found";
                                                var timestamp = (parseInt(userData.timestamp, 10) + 1695000000) * 1000;
                                                var currentDate = new Date();
                                                var currenttimestamp = currentDate.getTime();

                                                const date = new Date(timestamp);
                                                let values;
                                                if (!(userData.userreg in dynamicObject)) {
                                                    dynamicObject[userData.userreg] = ["In", timestamp];
                                                }
                                                else {
                                                    values = dynamicObject[userData.userreg];
                                                    if (values[0] == "Out" || (values[0] == "In" && (timestamp - values[1]) > 43200000)) { // 12 hours - 43200000

                                                        dynamicObject[userData.userreg] = ["In", timestamp];
                                                    }
                                                    else {
                                                        dynamicObject[userData.userreg] = ["Out", timestamp];
                                                    }
                                                }
                                                values = dynamicObject[userData.userreg];
                                                rowhtml += "<tr class='tbl_row'><td class='tbl_data'>";
                                                rowhtml += sn;
                                                rowhtml += "</td><td class='tbl_data'>";
                                                rowhtml += date;
                                                rowhtml += "</td><td class='tbl_data'>" + values[0] + "</td></tr>";
                                                sn++;
                                                rowshtml = rowhtml + rowshtml;
                                                // console.log("Data:", timestampData['username']);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if (htmlt == "found") {
                            html += (headhtml + rowshtml);
                            html += "</tbody></table></div></div></div>";
                            document.getElementById("tab_container").innerHTML = html;
                        }
                    }
                }
                if (html == "") {
                    document.getElementById("waitmsg").innerHTML = "No Data to Show 👾";
                }
            });

        }

        setInterval(() => {
            load_details();
        }, 3000);

        load_details();

    </script>

</head>
<style>
</style>

<body style="padding:20px;">


    <div class="text-wrap p-4 p-lg-5 text-center d-flex align-items-center order-md-last" style="width:100%">
        <div class="text w-100">
            <h2>
                <?php echo $username; ?>
            </h2>
            <p>
                <?php echo $userreg; ?>
                <br>
                <?php
                if ($_SESSION['rfid'] == "-") {
                    echo "Not Approved Yet";
                } ?>
            </p>
            <a href="./dashboard.php" class="btn btn-white btn-outline-white" style="margin-top:10px;">Dash
                Board</a>
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
            <form action="main.php" method="POST" autocomplete="off">
                <input type="submit" class="btn btn-white btn-outline-white" style="margin-top:10px;" name="log_out"
                    value="Log Out">
            </form>
        </div>
    </div>

    <div class="tab_container" id="tab_container">
        <h2 id="waitmsg">Just a sec 🔃</h2>
    </div>
    </section>
    <script src="js/rfid.js"></script>

</body>

</html>