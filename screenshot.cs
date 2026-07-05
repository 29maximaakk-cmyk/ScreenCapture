// screenshot.cs
using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Windows.Forms;
using System.Threading;

class Screenshot
{
    static void Main(string[] args)
    {
        string output = null;
        int delay = 0;
        string region = null;
        for (int i = 0; i < args.Length; i++)
        {
            if (args[i] == "-o" && i+1 < args.Length)
                output = args[++i];
            else if (args[i] == "-d" && i+1 < args.Length)
                delay = int.Parse(args[++i]);
            else if (args[i] == "-r" && i+1 < args.Length)
                region = args[++i];
            else if (args[i] == "-h" || args[i] == "--help")
            {
                Console.WriteLine("Usage: screenshot [options]");
                Console.WriteLine("  -o <file>   Output file");
                Console.WriteLine("  -d <sec>    Delay in seconds");
                Console.WriteLine("  -r <x,y,w,h> Region");
                Console.WriteLine("  -h          Help");
                return;
            }
        }
        if (delay > 0)
        {
            Console.WriteLine($"Ожидание {delay} секунд...");
            Thread.Sleep(delay * 1000);
        }
        if (string.IsNullOrEmpty(output))
            output = $"screenshot_{DateTime.Now:yyyyMMdd_HHmmss}.png";

        try
        {
            Bitmap bitmap;
            if (!string.IsNullOrEmpty(region))
            {
                var parts = region.Split(',');
                if (parts.Length != 4)
                {
                    Console.WriteLine("Неверный формат области. Используйте x,y,w,h");
                    return;
                }
                int x = int.Parse(parts[0]);
                int y = int.Parse(parts[1]);
                int w = int.Parse(parts[2]);
                int h = int.Parse(parts[3]);
                bitmap = new Bitmap(w, h);
                using (Graphics g = Graphics.FromImage(bitmap))
                {
                    g.CopyFromScreen(x, y, 0, 0, new Size(w, h));
                }
            }
            else
            {
                var screen = Screen.PrimaryScreen.Bounds;
                bitmap = new Bitmap(screen.Width, screen.Height);
                using (Graphics g = Graphics.FromImage(bitmap))
                {
                    g.CopyFromScreen(0, 0, 0, 0, screen.Size);
                }
            }
            bitmap.Save(output, ImageFormat.Png);
            Console.WriteLine($"Скриншот сохранён в {output}");
        }
        catch (Exception e)
        {
            Console.WriteLine($"Ошибка: {e.Message}");
        }
    }
}
