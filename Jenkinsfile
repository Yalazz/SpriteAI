pipeline {
  agent any
  stages {
    stage('Engine (CMake)') {
      steps {
        dir('spriteai-engine') {
          sh 'cmake -S . -B build -DCMAKE_BUILD_TYPE=Release'
          sh 'cmake --build build -j'
        }
      }
    }
    stage('Desktop Editor (CMake)') {
      steps {
        dir('spriteai-editor-desktop') {
          sh 'cmake -S . -B build -DCMAKE_BUILD_TYPE=Release'
          sh 'cmake --build build -j'
        }
      }
    }
    stage('AI Service') {
      steps { dir('spriteai-services/ai') { sh 'python -m compileall app' } }
    }
    stage('Backend') {
      steps { dir('spriteai-services/backend') { sh 'mvn -q -DskipTests package || true' } }
    }
  }
}
