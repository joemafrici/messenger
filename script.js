document.addEventListener("DOMContentLoaded", function() {
    var form = document.getElementById("messageForm");
    form.addEventListener("submit", function(event) {
        event.preventDefault(); // Prevent default form submission
    })

    var messageInput = document.getElementById("message");
    var message = messageInput.value;

    // Send the message to the C server
    // what if I set this path to the path to the WSL server?
    fetch("http://localhost:8080/submit", {
        method: "POST",
        headers: {
            "Content-Type": "application/x-www-form-urlencoded"
        },
        body: "message=" + encodeURIComponent(message)
    })
    .then(function(response) {
        // Handle the response from the C server
        if (response.ok) {
            console.log("Message sent successfully");
            // Do something on success
        } else {
            console.error("Error:", response.statusText);
            // Do something on error
        }
    })
    .catch(function(error) {
        console.error("Error:", error);
        // Handle any errors that occurred during the request
    });
});