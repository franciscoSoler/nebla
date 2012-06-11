package vista.ventanas;

import java.awt.BorderLayout;
import java.awt.Color;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import eventos.PresionarBotonAction;

import vista.paneles.AreaDeDibujo;

public class VentanaPrincipal extends JFrame {

	private JPanel panelContenedor;
	private AreaDeDibujo areaDeDibujo;
	private static final long serialVersionUID = 1L;
	
	public VentanaPrincipal (String title) {
		this.inicializar(title);
	}

	public VentanaPrincipal () {
		this.inicializar("Titulo");
	}

	private void inicializar (String title) {
		this.setTitle(title);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setExtendedState(NORMAL);
		this.setLocationRelativeTo(null);
		this.getContentPane().setLayout(new BorderLayout());
		this.setSize(650, 650);
		this.setVisible(true);
		
		panelContenedor = new JPanel();
		panelContenedor.setBorder(new EmptyBorder(5, 5, 5, 5));
		panelContenedor.setLayout(null);
		setContentPane(panelContenedor);
		
		areaDeDibujo = new AreaDeDibujo();
		areaDeDibujo.setBounds(10, 100 , 630, 500);
		areaDeDibujo.setBorder(BorderFactory.createLineBorder(Color.black));
		areaDeDibujo.setLayout(null);
		areaDeDibujo.setBackground(Color.green);
		panelContenedor.add(areaDeDibujo);
		
		JLabel label = new JLabel("Prueba 1");
		label.setBounds(10, 10, 130, 75);
		label.setBackground(Color.blue);
		label.setOpaque(true);
		
		JButton button = new JButton("Boton START");
		button.setBounds(100, 10, 250, 75);
		button.addActionListener(new PresionarBotonAction(this.areaDeDibujo));
		this.add(button);
		
		panelContenedor.add(label);
		
		panelContenedor.updateUI();
		
	}
}