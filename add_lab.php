<?php
if (!isset($_SESSION)) {
    session_start();
}
if ((!isset($_SESSION["email"])) || (!isset($_SESSION["desig"]))) {
    echo "<script>window.location.href ='index.php';</script>";
}
if (($_SESSION["desig"] != "ADMIN") && ($_SESSION["desig"] != "LABADMIN")) {
    echo "<script>window.location.href ='index.php';</script>";
}
date_default_timezone_set('Asia/Kolkata');
?>

<!doctype html>
<html lang="en">

<head>
    <link href="rel.jpg?v=<?php echo rand(); ?>" rel="icon">

    <title>Add Lab - RFID</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <link href="https://fonts.googleapis.com/css?family=Lato:300,400,700,900&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">
    <link rel="stylesheet" href="css/rfid.css">
    <link rel="stylesheet" href="css/addon.css">

    <script type="module">
        // Import the functions you need from the SDKs you need
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
        let checklab = 0;
        function countLab() {
            return new Promise((resolve, reject) => {
                const logsRef = ref(database, 'rfid/labs');

                onValue(logsRef, (snapshot) => {
                    const logsData = snapshot.val();

                    if (logsData !== null) {
                        const numberOfNodes = Object.keys(logsData).length + 1;
                        // console.log('Number of nodes in logs:', numberOfNodes);
                        resolve(numberOfNodes);
                    } else {
                        // console.log('No data found in logs');
                        resolve(1);
                    }
                });
            });
        }

        async function getCountLab() {
            try {
                checklab = await countLab();
                document.getElementById("labnum").value = checklab;
                // console.log("Total Labs : " + checklab);
            } catch (error) {
                // console.error("Error in getCountLab:", error);
            }
        }

        // Call getCountLab
        getCountLab();
        var adPhone = '<?php echo $_SESSION['phone'] ?>';
        // console.log(adPhone);
        var labnum = '';
        var labname = '';

        async function signup() {
            if (labnameva() == true && labnumva() == true) {
                labnum = document.getElementById('labnum').value;
                labname = document.getElementById('labname').value;
                signInAnonymously(auth)
                    .then((userCredential) => {
                        //user.uid  
                        set(ref(database, 'rfid/labs/' + labnum), {
                            labnum: labnum,
                            labname: labname
                        }).then((success) => {
                            alert("Lab Added Successfully");
                            window.location.href = 'add_lab.php';
                        }).catch((error) => {
                            alert("Lab Update Failed: " + error.message);
                        });
                    })
                    .catch((error) => {
                        var errorCode = error.code;
                        var errorMessage = error.message;
                        alert("Error Code - " + errorCode);
                        alert("Error Message - " + errorMessage);
                    });
                signInAnonymously(auth)
                    .then((userCredential) => {
                        //user.uid  
                        set(ref(database, 'rfid/users/' + adPhone + '/labs/' + labnum), {
                            labnum: labnum,
                            labname: labname
                        }).then((success) => {
                            alert("Data Added Successfully");
                            window.location.href = 'index.php';

                        }).catch((error) => {
                            alert("Data couldn't be Added !!");
                        });
                    })
                    .catch((error) => {
                        var errorCode = error.code;
                        var errorMessage = error.message;
                        alert("Error Code - " + errorCode);
                        alert("Error Message - " + errorMessage);
                    });

            } else {
                console.log("Not Aprroved");
                document.getElementById("suberr").innerHTML = "❌ Not Aprroved";
            }
        }

        const conf = document.getElementById("submit");
        if (conf != null) {
            conf.addEventListener("click", signup);
        }


    </script>
    <script src="js/rfid.js"></script>

</head>
<style>

</style>

<body>


    <section class="ftco-section">
        <div class="container">
            <div class="row justify-content-center">
                <div class="col-md-12 col-lg-10">
                    <div class="wrap d-md-flex">
                        <div class="text-wrap p-4 p-lg-5 text-center d-flex align-items-center order-md-last">
                            <div class="text w-100">
                                <h2>RFID Attendance</h2>
                                <?php echo $_SESSION['desig']; ?>
                                </p>
                                <?php
                                if (($_SESSION["desig"] == "ADMIN") || ($_SESSION["desig"] == "LABADMIN")) {
                                    ?>
                                    <a href="./dashboard.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Dash Board</a>
                                    <a href="./index.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Home</a>
                                    <a href="./permit_lab.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Permit LAB</a>
                                    <a href="./all_users.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">All Users</a>


                                <?php }
                                if ($_SESSION['desig'] == "ADMIN") {
                                    ?>
                                    <a href="./allow_lab_creation.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Make LAB ADMIN</a>
                                    <a href="./make_admin.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Make
                                        MAIN ADMIN</a>
                                    <a href="./approve.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Approve
                                        User</a>
                                    <a href="./remove_user.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Remove
                                        User</a>
                                <?php } ?>
                            </div>
                        </div>

                        <div class="login-wrap p-4 p-lg-5" id="mainsection">
                            <div class="d-flex">
                                <div class="w-100">
                                    <h3 class="mb-4">Add Lab</h3>
                                </div>
                            </div>


                            <div class="form-group mb-3">
                                <label class="label" for="name">LAB NAME *</label><span id="rfidspan"
                                    class="err"></span>
                                <span id="rfidspans" class="suc"></span>
                                <input type="text" class="form-control" placeholder="Sub-Sec-Room eg. DSA AB2 UB1102"
                                    id="labname" name="labname" maxlength="45" autocomplete="off" onkeyup="labnameva()">

                            </div>
                            <div class="form-group mb-3">
                                <label class="label" for="name">Lab code for this period</label><span id="mospan"
                                    class="err"></span>
                                <span id="mospans" class="suc"></span>
                                <input type="number" class="form-control" placeholder="FILLED AUTOMATICALLY" id="labnum"
                                    name="labnum" disabled>

                            </div>
                            <div class="form-group">
                                <span id="suberr" class="err"></span>

                                <button class="form-control btn btn-primary submit px-3" id="submit" name="submit">Add
                                    Lab</button>
                            </div>
                            <div class="w-100 text-md-right">
                                * Required
                            </div>
                        </div>

                    </div>
                </div>
            </div>
        </div>
    </section>

</body>

</html>