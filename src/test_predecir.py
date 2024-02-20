from prediccion_texto import generar_ngrama, entrenar_modelo, predecir

def test_generar_ngrama():
    assert generar_ngrama(['Hola', 'estoy', 'probando'], 2) == [('Hola', 'estoy'), ('estoy', 'probando')]

def test_entrenar_modelo():
    texto = [['Hola', 'estoy', 'probando'], ['Esto', 'es', 'una', 'prueba']]
    modelo = entrenar_modelo(texto)
    assert modelo['Hola']['sig']['estoy'] == 1
    assert modelo['estoy']['prev']['Hola'] == 1
    assert modelo['es']['sig']['una'] == 1
    assert modelo['una']['prev']['es'] == 1

def test_predecir():
    texto = [['Hola', 'estoy', 'probando'], ['Esto', 'es', 'una', 'prueba']]
    modelo = entrenar_modelo(texto)
    assert predecir(['Hola', '_', 'probando'], modelo) == 'estoy'
    assert predecir(['Esto', 'es', '_'], modelo) == 'una'