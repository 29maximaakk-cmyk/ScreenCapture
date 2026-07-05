// screenshot.go
package main

import (
	"flag"
	"fmt"
	"image/png"
	"os"
	"strconv"
	"strings"
	"time"

	"github.com/kbinani/screenshot"
)

func main() {
	var output string
	var delay int
	var region string
	flag.StringVar(&output, "o", "", "Выходной файл")
	flag.IntVar(&delay, "d", 0, "Задержка в секундах")
	flag.StringVar(&region, "r", "", "Область x,y,w,h")
	flag.Parse()

	if delay > 0 {
		fmt.Printf("Ожидание %d секунд...\n", delay)
		time.Sleep(time.Duration(delay) * time.Second)
	}

	if output == "" {
		output = fmt.Sprintf("screenshot_%s.png", time.Now().Format("20060102_150405"))
	}

	var img *image.RGBA
	var err error
	if region != "" {
		parts := strings.Split(region, ",")
		if len(parts) != 4 {
			fmt.Fprintln(os.Stderr, "Неверный формат области. Используйте x,y,w,h")
			os.Exit(1)
		}
		x, _ := strconv.Atoi(parts[0])
		y, _ := strconv.Atoi(parts[1])
		w, _ := strconv.Atoi(parts[2])
		h, _ := strconv.Atoi(parts[3])
		img, err = screenshot.CaptureRect(image.Rect(x, y, x+w, y+h))
	} else {
		bounds := screenshot.GetDisplayBounds(0)
		img, err = screenshot.CaptureRect(bounds)
	}
	if err != nil {
		fmt.Fprintf(os.Stderr, "Ошибка захвата: %v\n", err)
		os.Exit(1)
	}

	file, err := os.Create(output)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Ошибка создания файла: %v\n", err)
		os.Exit(1)
	}
	defer file.Close()
	err = png.Encode(file, img)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Ошибка сохранения: %v\n", err)
		os.Exit(1)
	}
	fmt.Printf("Скриншот сохранён в %s\n", output)
}
