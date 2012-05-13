package actions;

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;

@SuppressWarnings("serial")
public class EscPressedAction extends AbstractAction {

	public EscPressedAction() {
		System.out.println("Action created");
	}
	
	public void actionPerformed(ActionEvent arg0) {
		System.out.println("Apreto ESCAPE");
	}
}
