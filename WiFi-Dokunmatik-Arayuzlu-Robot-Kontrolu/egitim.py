import pandas as pd
from sklearn.tree import DecisionTreeClassifier, export_text
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report
import matplotlib.pyplot as plt

# Veriyi yükle
data = pd.read_json("joystick_verileri.json")
X = data[['dx', 'dy']]
y = data['label']

# Eğitim/Test böl
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Model oluştur ve eğit
model = DecisionTreeClassifier(max_depth=4)
model.fit(X_train, y_train)

# Test verisiyle tahmin
y_pred = model.predict(X_test)
print("=== Sınıflandırma Raporu ===\n")
print(classification_report(y_test, y_pred))

# Ağacı görselleştir
plt.figure(figsize=(12, 8))
from sklearn.tree import plot_tree
plot_tree(model, filled=True, feature_names=['dx', 'dy'], class_names=model.classes_)
plt.title("Joystick Karar Ağacı")
plt.show()

# JS için if-else çıkarımı
print("\n=== JavaScript getDirection Fonksiyonu ===\n")

def convert_tree_to_ifelse(tree_text):
    lines = tree_text.strip().split('\n')
    js_code = "function getDirection(dx, dy) {\n"
    indent_stack = []

    for line in lines:
        indent = len(line) - len(line.lstrip())
        content = line.strip()
        
        while indent_stack and indent < indent_stack[-1]:
            indent_stack.pop()
            js_code += "  " * len(indent_stack) + "}\n"
        
        if "class:" in content:
            label = content.split("class: ")[-1]
            js_code += "  " * len(indent_stack) + f'return "{label}";\n'
        elif "<=" in content or ">" in content:
            if "<=" in content:
                var, val = content.split(" <= ")
                cond = f"{var.strip()} <= {val.strip()}"
            else:
                var, val = content.split(" > ")
                cond = f"{var.strip()} > {val.strip()}"
            js_code += "  " * len(indent_stack) + f"if ({cond}) {{\n"
            indent_stack.append(indent)
    
    while indent_stack:
        indent_stack.pop()
        js_code += "  " * len(indent_stack) + "}\n"

    js_code += "}\n"
    return js_code

tree_rules = export_text(model, feature_names=['dx', 'dy'])
js_ifelse_code = convert_tree_to_ifelse(tree_rules)
print(js_ifelse_code)
