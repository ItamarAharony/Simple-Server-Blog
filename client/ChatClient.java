import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.net.Socket;

public class ChatClient {

    private Socket socket;              // Socket to connect to the server
    private BufferedReader input;       // Reader for receiving messages from the server
    private PrintWriter output;         // Writer for sending messages to the server
    private JTextArea messageArea;      // Text area to display messages from the chat
    private JTextField inputField;      // Input field for typing and sending messages

    // Constructor to initialize the client connection and GUI
    public ChatClient(String address, int port) {
        try {
            // Establish the socket connection to the server
            socket = new Socket(address, port);
            
            // Initialize input and output streams for communication
            input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            output = new PrintWriter(socket.getOutputStream(), true);
            
            // Setup the graphical user interface
            setupGUI();

            // Start a thread to listen for incoming messages from the server
            new Thread(this::listenForMessages).start();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Method to set up the graphical user interface
    private void setupGUI() {
        JFrame frame = new JFrame("Chat Client");    // Create a new window for the chat client
        messageArea = new JTextArea(20, 40);         // Create a text area with 20 rows and 40 columns
        messageArea.setEditable(false);              // Make the text area non-editable
        JScrollPane scrollPane = new JScrollPane(messageArea); // Add a scroll pane for the message area
        inputField = new JTextField(40);             // Create an input field with 40 columns

        // Add an action listener to the input field to send messages when the Enter key is pressed
        inputField.addActionListener(e -> {
            String message = inputField.getText();
            if (!message.isEmpty()) {
                // Append "Me: " to the client's message and display it immediately
                messageArea.append("Me: " + message + "\n");

                // Send the message to the server
                output.println(message);

                // Clear the input field
                inputField.setText("");
            }
        });

        // Create a panel to hold the message area and input field
        JPanel panel = new JPanel();
        panel.setLayout(new BorderLayout());
        panel.add(scrollPane, BorderLayout.CENTER);  // Add the message area (with scroll pane) to the center
        panel.add(inputField, BorderLayout.SOUTH);   // Add the input field to the bottom (south) of the panel

        // Add the panel to the frame and configure the frame settings
        frame.getContentPane().add(panel);
        frame.pack();                                // Size the frame so all contents are at their preferred size
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // Close the application when the window is closed
        frame.setVisible(true);                      // Make the window visible
    }

    // Method to continuously listen for messages from the server
private void listenForMessages() {
    String message;
    try {
        // Keep reading messages from the server and append them to the message area
        while ((message = input.readLine()) != null) {
            // Check if the message is from the current client
            if (message.startsWith("client " + socket.getLocalPort())) {
                messageArea.append("Me: " + message.substring(message.indexOf(':') + 2) + "\n");
            } else {
                messageArea.append(message + "\n");
            }
        }
    } catch (IOException e) {
        e.printStackTrace();
    }
}


    // Main method to start the chat client
    public static void main(String[] args) {
        // Use SwingUtilities to start the client in the event-dispatching thread
        SwingUtilities.invokeLater(() -> new ChatClient("127.0.0.1", 12345));
    }
}
