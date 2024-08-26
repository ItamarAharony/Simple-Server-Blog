import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.net.Socket;

public class ChatClient {

    private Socket socket;
    private BufferedReader input;
    private PrintWriter output;
    private JTextArea messageArea;
    private JTextField inputField;
    private String clientName;

    public ChatClient(String address, int port) {
        try {
            socket = new Socket(address, port);
            input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            output = new PrintWriter(socket.getOutputStream(), true);
            setupGUI();

            // Start a thread to listen for incoming messages
            new Thread(this::listenForMessages).start();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void setupGUI() {
        JFrame frame = new JFrame("Chat Client");
        messageArea = new JTextArea(20, 40);
        messageArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(messageArea);
        inputField = new JTextField(40);

        inputField.addActionListener(e -> {
            String message = inputField.getText();
            if (!message.isEmpty()) {
                output.println(message);
                inputField.setText("");
            }
        });

        JPanel panel = new JPanel();
        panel.setLayout(new BorderLayout());
        panel.add(scrollPane, BorderLayout.CENTER);
        panel.add(inputField, BorderLayout.SOUTH);

        frame.getContentPane().add(panel);
        frame.pack();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
    }

    private void listenForMessages() {
        String message;
        try {
            while ((message = input.readLine()) != null) {
                messageArea.append(message + "\n");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new ChatClient("127.0.0.1", 12345));
    }
}

