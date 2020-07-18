pipeline {
    agent any

    stages {
		stage('B')
        stage('Build') {
			agent {
				docker { image 'conanio/gcc10' }
			}
            steps {
                sh 'echo Hello World > testArtifact.rtf'
				sh 'conan install -if build . -s build_type=Release'
				archiveArtifacts artifacts: 'testArtifact.rtf', fingerprint: true
            }
        }
    }
}