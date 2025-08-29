(() => {
    const fs = require('fs');

    let contentSta = false;
    let contentEnd = false;

    fs.readFileSync('./CHANGELOG.md', { encoding: 'utf8' })
        .split(/\r\n|\n/)
        .forEach((line) => {
            if (contentEnd)
                return;
            if (contentSta) {
                if (/^\s*\*{3}\s*$/.test(line)) { // end
                    contentEnd = true;
                } else {
                    console.log(line);
                }
            } else if (/^\s*###\s*\[v.*?\]/.test(line)) { // start
                contentSta = true;
                console.log(line);
            }
        });
})();