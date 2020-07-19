pipeline {
    agent any

    stages {
        stage('Build') {
			agent {
				docker { image 'conanio/gcc10' }
			}
            steps {
                sh 'echo Hello World > testArtifact.rtf'
				sh 'conan create .'
				archiveArtifacts artifacts: 'testArtifact.rtf', fingerprint: true
            }
        }
    }
}