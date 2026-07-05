# screenshot.py
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import os
import argparse
from PIL import ImageGrab
import time
from datetime import datetime

def main():
    parser = argparse.ArgumentParser(description="ScreenCapture – утилита для скриншотов")
    parser.add_argument('-o', '--output', help='Выходной файл')
    parser.add_argument('-d', '--delay', type=int, default=0, help='Задержка в секундах')
    parser.add_argument('-r', '--region', help='Область x,y,w,h')
    args = parser.parse_args()

    if args.delay > 0:
        print(f"Ожидание {args.delay} секунд...")
        time.sleep(args.delay)

    # Определяем выходной файл
    if args.output:
        output_file = args.output
    else:
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        output_file = f"screenshot_{timestamp}.png"

    # Захват
    try:
        if args.region:
            x, y, w, h = map(int, args.region.split(','))
            bbox = (x, y, x+w, y+h)
            img = ImageGrab.grab(bbox=bbox)
        else:
            img = ImageGrab.grab()
        img.save(output_file)
        print(f"Скриншот сохранён в {output_file}")
    except Exception as e:
        print(f"Ошибка: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == '__main__':
    main()
