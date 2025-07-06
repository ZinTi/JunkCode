package com.example;

import com.example.ui.UserInterface;

import javax.swing.*;

public class App {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            try {
                UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            } catch (Exception e) {
                e.printStackTrace();
            }

            UserInterface ui = new UserInterface();
            ui.setVisible(true);
        });
    }
}