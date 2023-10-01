document.addEventListener("DOMContentLoaded", function () {
    var form = document.getElementById("messageForm");
    var loginForm = document.getElementById("loginForm");
    loginForm.addEventListener("submit", function (event) {
        event.preventDefault();
        var username = document.getElementById("username").value;
        console.log("username: " + username);
        var password = document.getElementById("password").value;
        console.log("password: " + password);
        fetch("http://localhost:8080/", {
            method: "POST",
            headers: {
                "Content-Type": "application/x-www-form-urlencoded"
            },
            body: "username=" + encodeURIComponent(username) + " password=" + encodeURIComponent(password)
        })
            .then(function (response) {
                console.log(response);
                if (response.ok) {
                    console.log("Username and password sent successfully");
                    return response.text();
                } else {
                    const responseContainer = document.getElementById('responseContainer');
                    responseContainer.classList.add('error');
                    console.error("Error:", response.statusText);
                    throw new Error("Request failed");
                }
            })
            .then(function (responseText) {
                console.log("Server response:", responseText);
                const responseContainer = document.getElementById('responseContainer');
                responseContainer.textContent = responseText;
            })
            .catch(function (error) {
                console.error("Error:", error);
                const responseContainer = document.getElementById('responseContainer');
                responseContainer.textContent = error;
            })
    })
    form.addEventListener("submit", function (event) {
        event.preventDefault(); // Prevent default form submission

        var messageInput = document.getElementById("message");
        var message = messageInput.value;
        console.log(message);
        // Send the message to the C server
        // what if I set this path to the path to the WSL server?
        fetch("http://localhost:8080/", {
            method: "POST",
            headers: {
                "Content-Type": "application/x-www-form-urlencoded"
            },
            body: "message=" + encodeURIComponent(message)
        })
            .then(function (response) {
                // Handle the response from the C server
                if (response.ok) {
                    console.log("Message sent successfully");
                    return response.text(); // Extract response body as text
                    // Do something on success
                } else {
                    console.error("Error:", response.statusText);
                    throw new Error("Request failed");
                }
            })
            .then(function (responseText) {
                console.log("Server response:", responseText);
                // Do something with the response
            })
            .catch(function (error) {
                console.error("Error:", error);
                // Handle any errors that occurred during the request
            });
    });
});