#!/usr/bin/env sh

cat <<'EOF'
_на текущий момент статья находится в процессе написания, опубликованы 5/8 частей_

Все файлы можно взять [тут](https://github.com/vbezhenar/stm32-tutorial).

EOF

tail -n +2 README.md

cat <<'EOF'

---

EOF

cat 1-explore/README.md

cat <<'EOF'

---

EOF

cat 2-loop/README.md

cat <<'EOF'

---

EOF

cat 3-blink/README.md

cat <<'EOF'

---

EOF

cat 4-loopc/README.md

cat <<'EOF'

---

EOF

cat 5-blinkc/README.md

cat <<'EOF'

---

EOF
