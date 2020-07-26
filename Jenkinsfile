pipeline {
    agent any
    options {
        ansiColor('xterm')
    }
    stages {
        stage('Build') {
			agent {
				docker { image 'conanio/gcc10' }
			}
            steps {
                sh 'pwd'
            }
        }
    }
}