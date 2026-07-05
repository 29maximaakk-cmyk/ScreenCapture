// screenshot.java
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import javax.imageio.ImageIO;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.concurrent.TimeUnit;

public class screenshot {
    public static void main(String[] args) {
        String output = null;
        int delay = 0;
        String region = null;
        for (int i = 0; i < args.length; i++) {
            if (args[i].equals("-o") && i+1 < args.length) {
                output = args[++i];
            } else if (args[i].equals("-d") && i+1 < args.length) {
                delay = Integer.parseInt(args[++i]);
            } else if (args[i].equals("-r") && i+1 < args.length) {
                region = args[++i];
            } else if (args[i].equals("-h") || args[i].equals("--help")) {
                System.out.println("Usage: java screenshot [options]");
                System.out.println("  -o <file>   Output file");
                System.out.println("  -d <sec>    Delay in seconds");
                System.out.println("  -r <x,y,w,h> Region");
                System.out.println("  -h          Help");
                return;
            }
        }
        if (delay > 0) {
            System.out.println("Ожидание " + delay + " секунд...");
            try {
                TimeUnit.SECONDS.sleep(delay);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        if (output == null) {
            String ts = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
            output = "screenshot_" + ts + ".png";
        }

        try {
            Robot robot = new Robot();
            BufferedImage image;
            if (region != null) {
                String[] parts = region.split(",");
                if (parts.length != 4) {
                    System.err.println("Неверный формат области. Используйте x,y,w,h");
                    System.exit(1);
                }
                int x = Integer.parseInt(parts[0]);
                int y = Integer.parseInt(parts[1]);
                int w = Integer.parseInt(parts[2]);
                int h = Integer.parseInt(parts[3]);
                Rectangle rect = new Rectangle(x, y, w, h);
                image = robot.createScreenCapture(rect);
            } else {
                Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
                Rectangle screenRect = new Rectangle(screenSize);
                image = robot.createScreenCapture(screenRect);
            }
            ImageIO.write(image, "png", new File(output));
            System.out.println("Скриншот сохранён в " + output);
        } catch (Exception e) {
            System.err.println("Ошибка: " + e.getMessage());
            System.exit(1);
        }
    }
}
