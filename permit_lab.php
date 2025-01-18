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
date_default_timezone_set('Asia/Kathmandu');
?>

<!doctype html>
<html lang="en">

<head>
    <link href="rel.jpg?v=<?php echo rand(); ?>" rel="icon">

    <title>Approve - RFID</title>
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
        // import { getFirestore, doc, getDoc } from "https://www.gstatic.com/firebasejs/10.4.0/firebase-firestore.js";


        // TODO: Add SDKs for Firebase products that you want to use
        // https://firebase.google.com/docs/web/setup#available-libraries

        // Your web app's Firebase configuration
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
        // const db = getFirestore(app);
        // var found = "true";

        var mono = '-';
        var rfid = '-';

        async function checkPhone() {
            let moval = document.getElementById('mono').value;
            const dbRef = ref(database, `rfid/users/`);
            // // Query by email property
            const quer = query(dbRef, orderByChild('phone'), equalTo(moval));
            // async query
            try {
                const snapshot = await get(quer);
                if (snapshot.exists()) {
                    return false;
                } else {
                    document.getElementById('mospan').innerHTML = "❌ Not a User";
                    document.getElementById('mospans').innerHTML = '';
                    return true;
                }
            } catch (error) {
                console.error(error);
                return false;
            }
        }
        const movalidate = document.getElementById("mono");
        if (movalidate != null) {
            movalidate.addEventListener("change", checkPhone);
        }

        async function signup() {
            if (givenLabCode > 0) {
                document.getElementById("labidspan").innerHTML = "";
                document.getElementById("labidspans").innerHTML = "✅";
                let checkPh = await checkPhone();
                if (checkPh == false) {
                    mono = document.getElementById('mono').value;

                    signInAnonymously(auth)
                        .then((userCredential) => {
                            //user.uid  
                            set(ref(database, 'rfid/users/' + mono + '/labs/' + givenLabCode), {
                                labnum: givenLabCode.toString(),
                                labname: givenLabName
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
                    console.log("No Phone");
                }

            }
            else {
                document.getElementById("labidspan").innerHTML = "❌ SELECT A LAB";
                document.getElementById("labidspans").innerHTML = "";
            }
        }

        const conf = document.getElementById("submit");
        if (conf != null) {
            conf.addEventListener("click", signup);
        }
        var adPhone = '<?php echo $_SESSION['phone'] ?>';
        let dynamicLabs = {};
        function countLab() {
            return new Promise((resolve, reject) => {
                const logsRef = ref(database, 'rfid/users/' + adPhone + '/labs/');

                onValue(logsRef, (snapshot) => {
                    const logsData = snapshot.val();
                    if (logsData !== null) {
                        let labName = "";
                        let labname = "";
                        for (const nodeKey in logsData) {
                            // console.log(nodeKey);
                            if (logsData.hasOwnProperty(nodeKey)) {
                                const labData = ref(database, 'rfid/users/' + adPhone + '/labs/' + nodeKey);
                                onValue(labData, (snapshot) => {
                                    labName = snapshot.val();
                                    let labname = labName['labname'];
                                    dynamicLabs[nodeKey] = [labname, nodeKey];
                                });
                            }
                        }
                        resolve(1);
                    } else {
                        resolve(1);
                    }
                });
            });
        }
        async function getCountLab() {
            try {
                await countLab();
                var selected_content = "";
                var selected_content = '<option value="-1"> ---- SELECT ---- </option>';
                for (const key in dynamicLabs) {
                    if (dynamicLabs.hasOwnProperty(key)) {
                        const value = dynamicLabs[key];
                        selected_content += `<option value="${value[1]}">${value[0]}</option>`;
                    }
                }
                // selected_content += '</select >';

                document.getElementById("selectLast").innerHTML = selected_content;
            } catch (error) {
                console.error("Error in getCountLab:", error);
            }
        }
        getCountLab();
        let givenLabCode = -1;
        let givenLabName = "-";
        function handleSelectChange(event) {
            givenLabCode = event.target.value;
            givenLabCode = parseInt(givenLabCode, 10);
            givenLabName = dynamicLabs[givenLabCode];
            givenLabName = givenLabName[0];
            // console.log(givenLabCode + givenLabName);
            if (givenLabCode > 0) {
                document.getElementById("labidspan").innerHTML = "";
                document.getElementById("labidspans").innerHTML = "✅";
            }
            else {
                document.getElementById("labidspan").innerHTML = "❌ SELECT A LAB";
                document.getElementById("labidspans").innerHTML = "";
            }
            // console.log(givenLabCode);
        }

        // Get the select element
        var selectElement = document.getElementById('selectLast');
        selectElement.addEventListener('change', handleSelectChange);
    </script>
    <script src="js/rfid.js"></script>

</head>
<style>
    select {
        margin: 10px;
        padding: 8px;
        border-radius: 5px;
        width: 90%;
        font-size: 16px;
    }
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
                                    <a href="./add_lab.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Add Lab</a>
                                    <a href="./all_users.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">All Users</a>
                                <?php }
                                if ($_SESSION['desig'] == "ADMIN") {
                                    ?>
                                    <a href="./allow_lab_creation.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Make LAB ADMIN</a>
                                    <a href="./make_admin.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Make ADMIN</a>
                                    <a href="./approve.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Approve User</a>
                                    <a href="./remove_user.php" class="btn btn-white btn-outline-white"
                                        style="margin-top:10px;">Remove User</a>
                                <?php } ?>
                            </div>
                        </div>

                        <div class="login-wrap p-4 p-lg-5" id="mainsection">
                            <div class="d-flex">
                                <div class="w-100">
                                    <h3 class="mb-4">PERMIT LAB</h3>
                                </div>
                            </div>

                            <div class="form-group mb-3">
                                <label class="label" for="name">Phone (WhatsApp)*</label>
                                <span id="mospan" class="err"></span>
                                <span id="mospans" class="suc"></span>
                                <input type="number" class="form-control"
                                    placeholder="With Country Code eg. 91**********" id="mono" name="number"
                                    autocomplete="off" onchange="mova()">
                            </div>
                            <div class="form-group mb-3">
                                <label class="label" for="name">Choose Lab*</label>
                                <span id="labidspan" class="err"></span>
                                <span id="labidspans" class="suc"></span><br>
                                <div class="select_container" id="select_container">
                                    <select id="selectLast" name="selectLast">
                                        <option value="-1"> Hold tight! Getting Labs ..</option>
                                    </select>
                                </div>
                            </div>
                            <div class="form-group">
                                <span id="suberr" class="err"></span>

                                <button class="form-control btn btn-primary submit px-3" id="submit"
                                    name="submit">Update RFID</button>

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