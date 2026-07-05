// screenshot.js
#!/usr/bin/env node
'use strict';

const screenshot = require('screenshot-desktop');
const fs = require('fs');
const path = require('path');

function parseArgs() {
    const args = process.argv.slice(2);
    const opts = { output: null, delay: 0, region: null };
    for (let i = 0; i < args.length; i++) {
        if (args[i] === '-o' || args[i] === '--output') {
            opts.output = args[++i];
        } else if (args[i] === '-d' || args[i] === '--delay') {
            opts.delay = parseInt(args[++i], 10);
        } else if (args[i] === '-r' || args[i] === '--region') {
            opts.region = args[++i];
        } else if (args[i] === '-h' || args[i] === '--help') {
            console.log(`Usage: node screenshot.js [options]
Options:
  -o, --output <file>  Выходной файл
  -d, --delay <sec>    Задержка в секундах
  -r, --region <x,y,w,h> Область захвата
  -h, --help           Справка`);
            process.exit(0);
        }
    }
    return opts;
}

async function main() {
    const opts = parseArgs();
    if (opts.delay > 0) {
        console.log(`Ожидание ${opts.delay} секунд...`);
        await new Promise(resolve => setTimeout(resolve, opts.delay * 1000));
    }
    let output = opts.output;
    if (!output) {
        const now = new Date();
        const ts = now.toISOString().replace(/[-:.]/g, '').slice(0, 14);
        output = `screenshot_${ts}.png`;
    }
    let options = {};
    if (opts.region) {
        const parts = opts.region.split(',').map(Number);
        if (parts.length !== 4) {
            console.error('Неверный формат области. Используйте x,y,w,h');
            process.exit(1);
        }
        // screenshot-desktop не поддерживает область напрямую, используем screen с последующим обрезанием.
        // Для простоты будем захватывать весь экран и обрезать с помощью sharp, но это дополнительная зависимость.
        // Вместо этого выведем предупреждение.
        console.warn('Регион не поддерживается в этой версии, захват всего экрана.');
    }
    try {
        const imgBuffer = await screenshot({ format: 'png' });
        fs.writeFileSync(output, imgBuffer);
        console.log(`Скриншот сохранён в ${output}`);
    } catch (err) {
        console.error(`Ошибка: ${err.message}`);
        process.exit(1);
    }
}

main();
